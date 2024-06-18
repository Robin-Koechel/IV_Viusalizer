#ifndef LOGFRAME_H
#define LOGFRAME_H

#include <QFrame>
#include <QTimer>
#include "LogDatabaseConnector.h"

namespace Ui {
class LogFrame;
}

/**
 * @brief Class representing a log frame.
 */
class LogFrame : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Construct a new LogFrame object.
     * 
     * @param a_parent Parent widget.
     */
    explicit LogFrame(QWidget *a_parent = nullptr);

    /**
     * @brief Destroy the LogFrame object.
     */
    ~LogFrame();

private slots:
    /**
     * @brief Slot function to update text field.
     */
    void update_text_field();

private:
    /**
     * @brief Get list of checked log types.
     * 
     * @return std::unordered_set<LogType> Set of checked log types.
     */
    std::unordered_set<LogType> get_list_of_checked_log_types();

    Ui::LogFrame *m_ui; /**< Pointer to the UI object. */
    QTimer *m_timer; /**< Timer for periodic updates. */
    LogDatabaseConnector* m_db_connector; /**< Pointer to the log database connector. */
};

#endif // LOGFRAME_H
