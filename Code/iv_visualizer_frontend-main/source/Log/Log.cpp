#include "Log.h"
#include <QString>

// Constructor
Log::Log(const LogType& a_log_type, const QString& a_description, const uint64_t a_timestamp)
    : m_log_type(a_log_type), m_description(a_description), m_timestamp(a_timestamp) {}

// Destructor
Log::~Log() {}

// Accessors
const QString& Log::get_description() const {
    return m_description;
}

const uint64_t Log::get_timestamp() const {
    return m_timestamp;
}

const LogType& Log::get_log_type() const {
    return m_log_type;
}
