#ifndef LOGDATABASECONNECTOR_H
#define LOGDATABASECONNECTOR_H

#include <QString>
#include "Log.h"
#include "LogType.h"
#include <unordered_set>

/**
 * @brief Class for connecting to a log database.
 */
class LogDatabaseConnector
{
private:
    //maybe change | to %7C

    const QString m_start = "{~=\""; /**< Start tag for query building. */ // maybe change :{~= to {~%3D
    const QString m_end = "\"}";     /**< End tag for query building. */
    const QString m_debug = "|debug"; /**< Debug log type tag. */
    const QString m_detector = "|detector"; /**< Detector log type tag. */
    const QString m_error = "|error"; /**< Error log type tag. */
    const QString m_postprocessor = "|postprocessor"; /**< Postprocessor log type tag. */
    const QString m_preprocessor = "|preprocessor"; /**< Preprocessor log type tag. */
    const QString m_warning = "|warning"; /**< Warning log type tag. */
    const QString m_limit = "10"; /**< Limit for query. */
    QString m_path; /**< Path for query. */
    QString m_base_url; /**< Base URL for database connection. */

    /**
     * @brief Generate query based on log types.
     * 
     * @param a_types Set of log types.
     * @return QString Generated query string.
     */
    QString generate_query(const std::unordered_set<LogType>* a_types);

    /**
     * @brief Make request to database.
     * 
     * @param query Query string.
     * @param a_start Start timestamp.
     * @return QString Response from database.
     */
    QString make_request(const QString& query, const uint64_t a_start);

    /**
     * @brief Get logs from JSON data.
     * 
     * @param a_logsAsJson JSON string containing logs.
     * @return QVector<Log*> Vector of log pointers.
     */
    QVector<Log*> get_logs(QString& a_logsAsJson);

public:
    /**
     * @brief Construct a new Log Database Connector object.
     * 
     * @param a_baseUrl Base URL for database connection.
     */
    LogDatabaseConnector();

    /**
     * @brief Destroy the Log Database Connector object.
     */
    ~LogDatabaseConnector();

    /**
     * @brief Get log data from database.
     * 
     * @param a_types Set of log types.
     * @param a_start Start timestamp.
     * @return QVector<Log*> Vector of log pointers.
     */
    QVector<Log*> get_log_data(const std::unordered_set<LogType>& a_types, uint64_t a_start);

    /**
     * @brief get the base url of the Loki db server
     * 
     * @return QString base url of the Loki db server
    */
    QString get_base_url();

};

#endif // LOGDATABASECONNECTOR_H
