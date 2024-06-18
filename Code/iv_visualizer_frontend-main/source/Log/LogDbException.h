#ifndef CUSTOM_EXCEPTION_H
#define CUSTOM_EXCEPTION_H

#include <exception>
#include <QString>


class LogDbException : public std::exception {
public:
    LogDbException(const char* message);
    virtual const char* what() const noexcept override;

private:
    const char* m_msg;
};

#endif // CUSTOM_EXCEPTION_H
