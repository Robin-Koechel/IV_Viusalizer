#ifndef VIDEO_PLAYER_WIDGET_H
#define VIDEO_PLAYER_WIDGET_H

#include "AskingAfterStreamThread.h"
#include "../DataStreams/DataFrameStream.h"
#include "../BackendConnector/Stream.h"
#include "VideoNavigator.h"
#include <QFrame>
#include "VideoPlayer.h"
#include "../ViewType/ViewType.h"
#include "../ViewType/ViewTypeFrameWidget.h"
#include <QThread>
#include <QTimer>

namespace Ui {
class VideoPlayerWidget;
}

/**
 * @class VideoPlayerWidget
 * @brief Widget to play and navigate through video streams.
 *
 * This widget integrates various components to facilitate the playback
 * and navigation of video streams, including live and recorded data.
 * It allows changing streams, adjusting playback speed, and viewing specific
 * frames within a stream.
 */
class VideoPlayerWidget : public QFrame
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a VideoPlayerWidget object.
     * @param a_parent The parent widget.
     */
    explicit VideoPlayerWidget(QWidget* a_parent = nullptr);

    /**
     * @brief Destroys the VideoPlayerWidget object.
     */
    ~VideoPlayerWidget();

    /**
     * @brief Retrieves the next image from the video stream.
     * @return A pointer to the QGraphicsScene containing the next image.
     */
    QGraphicsScene* get_next_image();

    /**
     * @brief stops the showing stream
    */
    void stop_stream();


    /**
     * Handles key press events to navigate through the video.
     * @param a_event The key event that triggered the navigation.
     */
    void keyPressEvent(QKeyEvent *a_event) override;

    /**
     * @brief Changes the current video stream.
     * @param a_new_stream The new video stream.
     */
    void change_stream(Stream* a_new_stream);

    /**
     * @brief Retrieves the current video stream.
     * @return The current video stream.
     */
    Stream* get_stream();


    /*
    * @brief marks the naviagtor on specific positions
    * @param a_positions the positions to mark
    */
    void set_marks(QList<uint64_t> &a_positions);

    /*
    * @brief gets the current selected ViewType
    * @return selected ViewType
    */
    const ViewType& get_view_type();

    /**
     * @brief disablelive
    */
   void disablelive();


private:
 

    /**
     * @brief Starts the timer for frame retrieval.
     */
    void start_timer();

    /**
     * @brief Retrieves a new data frame stream.
     * @return A pointer to the new data frame stream.
     */
    DataFrameStream* get_new_stream(); // Not live

    /**
     * @brief Sends a signal to request a new data frame stream.
     */
    void send_signal_for_new_data_frame_stream();



signals:
    /**
     * @brief Signal emitted when the data frame stream needs to be changed.
     * @param a_new_stream The new data frame stream.
     * @param a_current_position The current position within the stream.
     */
    void sig_change_data_frame_stream(DataFrameStream* a_new_stream, uint64_t a_current_position);

    /**
     * @brief Signal emitted when the playback speed needs to be changed.
     * @param a_speed The new playback speed.
     */
    void sig_change_speed(double a_speed);

    /**
     * @brief Signal emitted to toggle between play and pause states.
     * @param a_playing says if it is on pause or playing
     */
    void sig_play_or_pause(bool a_playing);

    /**
     * @brief Signal emitted when the playback position changes.
     * @param a_position_changed The new playback position.
     */
    void sig_position_changed(uint64_t a_position_changed);

    /**
     * @brief Signal emitted to request a new data frame stream.
     * @param a_current_position The current position within the stream.
     * @param a_type The view type.
     */
    void sig_ask_for_new_data_frame_stream(uint64_t a_current_position, const ViewType* a_type);

    /**
     * @brief Signal emitted to request a new live data frame stream.
     * @param a_current_position The current position within the stream.
     * @param a_type The view type.
     */
    void sig_ask_for_new_live_data_frame_stream(uint64_t a_current_position, const ViewType* a_type);

    /**
     * @brief Signal emitted to notify about a new stream.
     * @param a_stream The new stream.
     */
    void sig_new_stream(Stream* a_stream);

    /**
     * Navigates one frame forward in the video stream.
     */
    void sig_one_frame_forwards();

    /**
     * Navigates one frame backward in the video stream.
     */
    void sig_one_frame_backwards();

    /**
     * @brief Signal emitted to notify the slider moved
     * @param a_new_position the position of the navigator
    */
    void sig_navigator_moved(uint64_t a_new_position);

 


public slots:
    /**
     * @brief Slot to change the current playback position.
     * @param a_new_position The new playback position.
     */
    void slot_change_current_position(uint64_t a_new_position);

    /**
     * @brief Slot to handle changes in playback position.
     * @param a_new_position The new playback position.
     */
    void slot_pos_changed(uint64_t a_new_position);

    /**
     * @brief Slot to request a new data frame stream.
     */
    void slot_ask_for_new_data_frame_stream();

    /**
     * @brief Slot to show a data frame.
     * @param a_data_frame_to_show The data frame to show.
     * @param a_blocking_pos Flag indicating blocking position.
     * @param a_version The version of the data frame.
     */
    void slot_show_data_frame(const DataFrame* a_data_frame_to_show, bool a_blocking_pos, int a_version);

    /**
     * @brief Slot to handle changes in view type.
     * @param a_type The new view type.
     */
    void slot_view_type_changed(const ViewType& a_type);

    /**
     * @brief Slot to send a new data frame stream.
     * @param a_new_stream The new data frame stream.
     */
    void slot_send_new_data_frame_stream(DataFrameStream* a_new_stream);

private slots:
    /**
     * @brief Slot triggered when the play button is clicked.
     */
    void on_btnPlay_clicked();

    /**
     * @brief Slot triggered when the speed selector's current text is changed.
     * @param a_arg1 The new text.
     */
    void on_speedSelector_currentTextChanged(const QString& a_arg1);

    /**
     * @brief Slot triggered when the live checkbox's state is changed.
     * @param a_arg1 The new state.
     */
    void on_checkBoxLive_stateChanged(int a_arg1);

private:
    const int convert_ms_to_ns = 1000000; ///<factor to convert sec to ms
    const int convert_sec_to_ns = 1000000000; ///<factor to convert sec to ns
    const QString m_settings_go_back_in_ms = "AskingIntoThePastInMs";
    const QString m_settings_time_of_fine_slider_in_sec = "TimeSpanOfFineSliderInSec";
    const QString m_settings_go_forward_in_ms = "AskingIntoTheFutureInMs"; 
    const QString m_play = "Play";
    const QString m_stop = "Stop";
    const QString settings_group_name = "VideoPlayerWidget/"; ///<Groupname of settings
    const uint64_t buffer_size = 0;

    Ui::VideoPlayerWidget* m_ui; ///< User interface pointer.
    VideoNavigator* m_navigator; ///< Video navigator pointer.
    ViewTypeFrameWidget* m_view_type_frame; ///< View type frame widget pointer.
    Stream* m_current_stream; ///< Current video stream.
    double m_current_speed; ///< Current playback speed.
    bool m_is_live; ///< Flag indicating live stream.
    VideoPlayer* m_player; ///< Video player pointer.
    QThread* m_thread_of_video_player; ///< Video player thread.
    AskingAfterStreamThread* m_thread_of_asking_stream; ///< Thread for asking after stream.
    bool m_blocking_pos_changed; ///< Flag indicating blocking position change.
    PipelineData m_pipe_data; ///< Pipeline data.
    PipelineObject* m_pipe_object; ///< Pipeline object.
    int m_current_version; ///< Current version of DataFrames.
    TimeRange m_current_time_range_of_accessible_data; ///< current timerange of accesible data
    bool m_use_mock; ///< a flag to signal if mpck is used
    bool m_live_changed; ///< signals if there was a live change

};

#endif // VIDEO_PLAYER_WIDGET_H
