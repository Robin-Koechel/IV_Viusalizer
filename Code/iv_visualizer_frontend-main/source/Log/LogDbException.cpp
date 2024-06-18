#include "LogDbException.h"

// Constructor implementation
LogDbException::LogDbException(const char* a_message) : m_msg(a_message) {}

// Override what() method to provide a message
const char* LogDbException::what() const noexcept {
    return m_msg;
}
