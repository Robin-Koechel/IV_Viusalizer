#include "LogFrame.h"
#include "../../build/ui_logframe.h"
#include "LogType.h"
#include <QDebug>
#include "LogDbException.h"
#include <QSettings>
#include "../main/MainWindow.h"
LogFrame::LogFrame(QWidget *a_parent)
    : QFrame(a_parent),
      m_ui(new Ui::LogFrame)
{
    m_ui->setupUi(this);

    QString a_settings_path = MainWindow::m_config_file_path;
    QSettings a_settings(a_settings_path, QSettings::IniFormat);
    QString section = "loki";
    m_db_connector = new LogDatabaseConnector(); 

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &LogFrame::update_text_field);

    QString key = "request_frequency";
    m_timer->start(a_settings.value(section + "/" + key).toInt());
}

LogFrame::~LogFrame()
{
    delete m_ui;
    delete m_db_connector;
    delete m_timer; // Use correct member variable name
}

void LogFrame::update_text_field()
{
    try {
        // Get the current time point
        auto a_current_time_point = std::chrono::system_clock::now();

        // Subtract 5 seconds from the current time point
        auto a_five_seconds_ago = a_current_time_point - std::chrono::seconds(5); // TODO: Load duration from settings.yaml

        // Convert the time point to uint64_t (Unix timestamp)
        uint64_t a_current_timestamp = std::chrono::duration_cast<std::chrono::seconds>(a_five_seconds_ago.time_since_epoch()).count();

        QVector<Log*> a_logs = m_db_connector->get_log_data(get_list_of_checked_log_types(), a_current_timestamp);

        for (Log* a_log : a_logs) {
            if (a_log) {
                m_ui->logTxtArea->appendPlainText(a_log->get_description() + "\n");
            } else {
                qDebug() << "Null log object encountered."; 
            }
        }
    } catch (LogDbException ex) {
        m_ui->logTxtArea->document()->clear();
        m_ui->logTxtArea->document()->setPlainText(ex.what());
    }
}

std::unordered_set<LogType> LogFrame::get_list_of_checked_log_types()
{
    std::unordered_set<LogType> a_log_types;

    if (m_ui->debugCBox->isChecked()) {
        a_log_types.insert(LogType::DEBUG);
    }

    if (m_ui->postprocessorCBox->isChecked()) {
        a_log_types.insert(LogType::POSTPROCESSOR);
    }

    if (m_ui->preprocessorCBox->isChecked()) {
        a_log_types.insert(LogType::PREPROCESSOR);
    }

    if (m_ui->detectorCBox->isChecked()) {
        a_log_types.insert(LogType::DETECTOR);
    }

    if (m_ui->errorCBox->isChecked()) {
        a_log_types.insert(LogType::ERROR);
    }

    if (m_ui->warningCBox->isChecked()) {
        a_log_types.insert(LogType::WARNING);
    }

    return a_log_types;
}
