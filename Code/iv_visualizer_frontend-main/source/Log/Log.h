#ifndef LOG_H
#define LOG_H

#include <QString>
#include "LogType.h"

/**
 * @brief Class representing a log entry.
 */
class Log {
private:
    QString m_description; /**< Description of the log entry. */
    LogType m_log_type;    /**< Type of the log entry. */
    uint64_t m_timestamp;  /**< Timestamp of the log entry. */

public:
    /**
     * @brief Construct a new Log object.
     * 
     * @param m_log_type Type of the log.
     * @param m_description Description of the log.
     * @param m_timestamp Timestamp of the log.
     */
    Log(const LogType& m_log_type, const QString& m_description, const uint64_t m_timestamp);

    /**
     * @brief Destroy the Log object.
     */
    ~Log();

    /**
     * @brief Get the description of the log.
     * 
     * @return const QString& Description of the log.
     */
    const QString& get_description() const;

    /**
     * @brief Get the type of the log.
     * 
     * @return const LogType& Type of the log.
     */
    const LogType& get_log_type() const;

    /**
     * @brief Get the timestamp of the log.
     * 
     * @return const uint64_t timestamp of the log.
     */
    const uint64_t get_timestamp() const;

};

#endif // LOG_H
