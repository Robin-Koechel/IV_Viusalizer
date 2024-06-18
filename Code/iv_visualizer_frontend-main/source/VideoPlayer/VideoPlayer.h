#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include "../DataStreams/DataFrameStream.h"
#include "../ViewType/ViewType.h"
#include <QFrame>
#include <QThread>
#include <QTimer>

/**
 * @class VideoPlayer
 * @brief Manages video playback, including play, pause, and stream handling functionalities.
 *
 * VideoPlayer is responsible for controlling video playback operations such as playing and pausing video streams,
 * handling speed changes, and updating the current video frame. It interacts with a DataFrameStream to retrieve
 * video data and utilizes Qt's event system to manage playback state and timing.
 */
class VideoPlayer : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructs a VideoPlayer object.
     * @param a_buffer_time The time in milliseconds to buffer video data.
     */
    explicit VideoPlayer(uint64_t a_buffer_time);

private:
    /**
     * Stops the video playback.
     */
    void stop_video();

    /**
     * Starts or resumes the video playback.
     */
    void play_video();

    /**
     * Stops the data frame stream associated with the video player.
     */
    void stop_data_frame_stream();

    /**
     * Displays the current data frame in the video stream.
     */
    void show_current_data_frame();

    /**
     * Starts the playback timer based on the current playback speed.
     */
    void start_timer();

    /**
     * Calculates the difference in milliseconds between two timestamps.
     * @param a_start The start timestamp.
     * @param a_end The end timestamp.
     * @return The difference in milliseconds.
     */
    uint64_t calculate_time_stamp_difference_in_ms(uint64_t a_start, uint64_t a_end);

private slots:
    /**
     * Slot triggered on playback timer timeout.
     */
    void slot_on_timer_timeout();

public slots:
    /**
     * Changes the playback speed of the video.
     * @param a_speed The new playback speed. Values > 1.0 increase speed, values < 1.0 decrease it.
     */
    void slot_speed_changed(double a_speed);

    /**
     * Toggles the play or pause state of the video.
     * @param a_playing says if stream is playing or not
     */
    void slot_play_or_pause(bool a_playing);

    /**
     * Changes the current data frame stream to a new stream and seeks to a specified position.
     * @param a_new_stream The new data frame stream.
     * @param a_position The position in the stream to seek to.
     */
    void slot_change_data_frame_stream(DataFrameStream* a_new_stream, uint64_t a_position);

    /**
     * Changes the current playback position in the video stream.
     * @param a_new_pos The new playback position in milliseconds.
     */
    void slot_pos_changed(uint64_t a_new_pos);

    /**
     * Navigates one frame forward in the video stream.
     */
    void slot_one_frame_forwards();

    /**
     * Navigates one frame backward in the video stream.
     */
    void slot_one_frame_backwards();


signals:
    /**
     * Signal emitted when the playback position changes.
     * @param a_new_pos The new playback position in milliseconds.
     */
    void sig_pos_changed(uint64_t a_new_pos);

    /**
     * Requests a new data frame stream.
     */
    void sig_ask_for_new_data_frame_stream();

    /**
     * Signal to display a specific data frame.
     * @param a_data_frame_to_show The data frame to be displayed.
     * @param a_blocking_pos Indicates whether the position change should be blocking.
     * @param a_version The version of the data frame to be displayed.
     */
    void sig_show_data_frame(const DataFrame* a_data_frame_to_show, bool a_blocking_pos, int a_version);

private:
    DataFrameStream* m_data_frame_stream = nullptr; ///< Pointer to the current data frame stream.
    DataFrame* m_current_data_frame;                ///< The current data frame being displayed.
    double m_current_speed;                       ///< The current playback speed.
    QTimer* m_timer;                             ///< Pointer to the playback timer.
    bool m_is_live;                               ///< Flag indicating if the stream is live.
    DataFrame* m_next_data_frame;                   ///< The next data frame to be displayed.
    TimeRange* m_current_time_range;               ///< Pointer to the current time range of the video.
    bool m_is_playing;                            ///< Flag indicating if the video is currently playing.
    uint64_t m_buffer_time;                       ///< Buffer time in ns.
    bool m_blocking_pos;                          ///< Flag indicating if the position change is blocking.
    int m_current_version = 0;                    ///< The current version of the data frame to display.
};

#endif // VIDEOPLAYER_H
