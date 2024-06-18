#ifndef ASKINGAFTERSTREAMTHREAD_H
#define ASKINGAFTERSTREAMTHREAD_H

#include <QThread>
#include "../BackendConnector/Stream.h"
#include "../ViewType/ViewType.h"

/**
 * @class AskingAfterStreamThread
 * @brief Thread for asynchronously requesting new data frame streams based on user navigation.
 *
 * This class extends QThread and is designed to handle requests for new data frame streams from a given stream source,
 * considering the time range the user wishes to navigate to, both in the future and in the past.
 */
class AskingAfterStreamThread : public QThread
{
    Q_OBJECT
public:
    /**
     * Constructs an asking after stream thread.
     * @param a_stream Pointer to the stream to query for new data frame streams.
     * @param a_time_to_go_in_the_future How far into the future (in milliseconds) the thread should query for data.
     * @param a_time_to_go_into_the_past How far into the past (in milliseconds) the thread should query for data.
     */
    AskingAfterStreamThread(Stream *a_stream, uint64_t a_time_to_go_in_the_future, uint64_t a_time_to_go_into_the_past);

public slots:
    /**
     * Requests a new data frame stream based on the current position and view type.
     * @param a_current_position The current position in the video stream.
     * @param a_type The type of view requested.
     */
    void slot_ask_for_new_data_frame_stream(uint64_t a_current_position, const ViewType* a_type);

    /**
     * Requests a new live data frame stream based on the current position and view type.
     * @param a_current_position The current position in the live video stream.
     * @param a_type The type of live view requested.
     */
    void slot_ask_for_new_live_data_frame_stream(uint64_t a_current_position, const ViewType* a_type);

    /**
     * Changes the stream source for data frame stream requests.
     * @param a_new_stream The new stream to set as the source.
     */
    void slot_change_stream(Stream* a_new_stream);

signals:
    /**
     * Signal emitted to change the data frame stream being used.
     * @param a_new_stream The new data frame stream to use.
     */
    void sig_change_data_frame_stream(DataFrameStream* a_new_stream);

private:
    /**
     * Retrieves a new stream based on the given position and view type (not live).
     * @param a_position The position to query the stream at.
     * @param a_type The view type to base the stream request on.
     * @return A new DataFrameStream based on the query parameters.
     */
    DataFrameStream* get_new_stream(uint64_t a_position, const ViewType& a_type);

    /**
     * Sends a signal to request changing to a new data frame stream.
     */
    void send_signal_for_new_data_frame_stream();

private:
    Stream* m_stream_to_ask; ///< The stream to query for new data.
    uint64_t m_time_to_go_in_the_future; ///< Time to look ahead for data in ns.
    uint64_t m_time_to_go_into_the_past; ///< Time to look back for data in ns.
    TimeRange m_current_time_range_of_accessible_data; ///< The current time range of accessible data.
};

#endif // ASKINGAFTERSTREAMTHREAD_H
