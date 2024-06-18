#ifndef VIDEONAVIGATOR_H
#define VIDEONAVIGATOR_H

#include <QFrame>
#include <QKeyEvent>

namespace Ui {
class VideoNavigator;
}

/**
 * @class VideoNavigator
 * @brief Provides navigation controls for video playback, such as moving one frame forward or backward.
 *
 * VideoNavigator is an abstract base class that defines the interface for navigating through a video stream.
 * It handles key press events to navigate one frame forward or backward and allows setting and getting
 * the current position in the video stream.
 */
class VideoNavigator : public QFrame
{
    Q_OBJECT
public:
    /**
     * Constructs a VideoNavigator with a specific time to navigate.
     * @param a_time_to_navigate The amount of time in milliseconds to navigate with each key press.
     * @param a_parent The parent widget.
     */
    explicit VideoNavigator(uint64_t a_time_to_navigate, QWidget *a_parent = nullptr);

    /**
     * Constructs a VideoNavigator without specifying the time to navigate.
     * @param a_parent The parent widget.
     */
    explicit VideoNavigator(QWidget *a_parent = nullptr);

    /**
     * Destroys the VideoNavigator.
     */
    ~VideoNavigator();

    /**
     * Sets the amount of time to navigate with each key press.
     * @param a_new_time_to_navigate The new time to navigate in milliseconds.
     */
    virtual void set_time_to_navigate(uint64_t a_new_time_to_navigate) = 0;

    /**
     * Retrieves the current position in the video stream.
     * @return The current position in milliseconds.
     */
    virtual uint64_t get_current_position() = 0;

    /**
     * Changes the current position in the video stream.
     * @param a_new_position The new position in milliseconds.
     */
    virtual void change_current_position(uint64_t a_new_position) = 0;

    /**
     * @brief marks the positions
     * @param a_positions marks to mark
     * 
    */
    virtual void set_marks(const QList<uint64_t> &a_positions) = 0;

signals:
    /**
     * Signal emitted when the position in the video stream changes.
     * @param a_new_position The new position in milliseconds.
     */
    void sig_pos_changed(uint64_t a_new_position);




private:
    Ui::VideoNavigator *m_ui; ///< The user interface for the VideoNavigator.

protected:
    uint64_t m_time_to_navigate; ///< The amount of time in milliseconds to navigate with each key press.
};

#endif // VIDEONAVIGATOR_H
