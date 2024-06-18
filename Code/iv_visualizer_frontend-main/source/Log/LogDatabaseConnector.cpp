#include "LogDatabaseConnector.h"
#include "../../include/json.hpp"
#include "httplib.h"
#include <iostream>
#include "Log.h"
#include <stdexcept>
#include <chrono>
#include <QVector>
#include "../main/MainWindow.h"
#include <QSettings>
#include "LogDbException.h"

using json = nlohmann::json;

LogType log_type_from_string(const QString& a_str) {
    if (a_str == "detector") {
        return LogType::DETECTOR;
    } else if (a_str == "preprocessor") {
        return LogType::PREPROCESSOR;
    } else if (a_str == "postprocessor") {
        return LogType::POSTPROCESSOR;
    } else if (a_str == "error") {
        return LogType::ERROR;
    } else if (a_str == "warning") {
        return LogType::WARNING;
    } else if (a_str == "debug") {
        return LogType::DEBUG;
    } else {
        throw std::invalid_argument("Unknown log type: " + a_str.toStdString());
    }
}

LogDatabaseConnector::LogDatabaseConnector(){
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);
    this->m_base_url = settings.value("loki/url").toString();
    this->m_path = settings.value("loki/path").toString();
}

QVector<Log*> LogDatabaseConnector::get_log_data(const std::unordered_set<LogType>& a_types, uint64_t a_start) {
    try{
        QString a_query = generate_query(&a_types);

        QString a_response = make_request(a_query, a_start);

        return get_logs(a_response);
    } catch(LogDbException ex){
        throw ex;
    }
}

QString LogDatabaseConnector::generate_query(const std::unordered_set<LogType>* a_types) {
    QString a_query = "";
    a_query.append(this->m_start);

    for (LogType a_log_type : *a_types) {
        switch (a_log_type) {
        case LogType::DEBUG:
            a_query.append(this->m_debug);
            break;
        case LogType::DETECTOR:
            a_query.append(this->m_detector);
            break;
        case LogType::ERROR:
            a_query.append(this->m_error);
            break;
        case LogType::POSTPROCESSOR:
            a_query.append(this->m_postprocessor);
            break;
        case LogType::PREPROCESSOR:
            a_query.append(this->m_preprocessor);
            break;
        case LogType::WARNING:
            a_query.append(this->m_warning);
            break;
        default:
            break;
        }
    }
    a_query.append(this->m_end);
    a_query.replace("\"|", "\"");
    return a_query;
}

QString LogDatabaseConnector::make_request(const QString& a_query, const uint64_t a_start) {
    httplib::Client a_client(this->m_base_url.toStdString().c_str());
    
    QString a_request = this->m_base_url + this->m_path + "?query=" + a_query + "&timestamp>" + QString::number(a_start);
    std::cout << a_request.toStdString() << std::endl;

    auto a_response = a_client.Get(a_request.toStdString().c_str());
    if (a_response && a_response->status == 200) {
        return QString::fromStdString(a_response->body);
    } else {
        throw LogDbException("Connection to the Loki database could not be established.");
    }
    
}


QVector<Log*> LogDatabaseConnector::get_logs(QString& a_logs_as_json) {
    QVector<Log*> a_logs;

    try {
        json a_response_json = json::parse(a_logs_as_json.toStdString());

        if (a_response_json.contains("data") && a_response_json["data"].contains("result")) {
            json a_result_array = a_response_json["data"]["result"];

            // Iterate over each stream in the result array
            for (auto& a_stream : a_result_array) {
                // Extract labels and log entries
                auto a_labels = a_stream["stream"];
                auto a_values = a_stream["values"];

                // Iterate over log entries in the stream
                for (auto& a_entry : a_values) {
                    int64_t a_timestamp = a_entry[0].get<int64_t>();
                    QString a_message = QString::fromStdString(a_entry[1].get<std::string>());
                    QString a_level = QString::fromStdString(a_labels["level"].get<std::string>());

                    // Create a Log object and add it to the vector
                    Log a_log(log_type_from_string(a_level), a_message, a_timestamp);
                    a_logs.push_back(&a_log);
                }
            }
        } else {
            throw std::invalid_argument("Invalid response log structure");
        }
    } catch (std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        throw std::invalid_argument("Error parsing JSON" + std::string(e.what()));
    }

    return a_logs;
}

QString LogDatabaseConnector::get_base_url(){
    return this->m_base_url;
}


LogDatabaseConnector::~LogDatabaseConnector(){
}


