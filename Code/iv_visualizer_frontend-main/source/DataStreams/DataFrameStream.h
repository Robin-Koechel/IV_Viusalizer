#ifndef DATA_FRAME_STREAM_H
#define DATA_FRAME_STREAM_H

#include "DataFrameRingBuffer.h"
#include "DataStream.h"
#include "../BackendConnector/TimeRange.h"

/**
 * @class DataFrameStream
 * @brief Manages the streaming of DataFrame objects within a given time range and buffers them in a ring buffer.
 *
 * This class interfaces with a DataStream to fetch DataFrame objects based on a specified time range,
 * managing these frames within a DataFrameRingbuffer for efficient access and storage.
 */
class DataFrameStream
{


public:
    /**
     * @brief Constructs a DataFrameStream object.
     * @param a_data_stream Pointer to the DataStream from which DataFrame objects are fetched.
     * @param a_end_time The end time for the time range of this stream.
     * @param a_size_of_ring_buffer The size of the ring buffer to hold DataFrame objects.
     */
    DataFrameStream(DataStream* a_data_stream, uint64_t a_end_time, int a_size_of_ring_buffer);

    /**
     * @brief Destructor for DataFrameStream.
     */
    ~DataFrameStream();

    /**
     * @brief Checks if the stream contains the previous dataframe.
     * @return True if the DataFrame exists, false otherwise.
     */
    bool is_previous_data_frame_in_stream();



    /**
     * @brief Retrieves the previous DataFrame from the ring buffer.
     * @return A pointer to the previous DataFrame in the stream.
     */
    DataFrame* get_previous_data_frame();


    /**
     * @brief Checks if the stream contains a DataFrame at the specified position.
     * @param a_position The position to check for a DataFrame.
     * @return True if the DataFrame exists at the specified position, false otherwise.
     */
    bool contains_data_frame(uint64_t a_position);

    /**
     * @brief Retrieves the DataFrame at the specified position as Timestamp.
     * @param a_position The position of the DataFrame to retrieve.
     * @return A constant reference to the DataFrame at the specified position.
     */
    DataFrame* get_data_frame(uint64_t a_position);

    /**
     * @brief Retrieves the next DataFrame from the ring buffer.
     * @param a_create_data_frame flag to say if dataframe gets created
     * @return A pointer to the next DataFrame in the stream if you say taht no dataframe shoud be created return nullptr.
     */
    DataFrame* get_next_data_frame(bool a_create_data_frame);

    /**
     * @brief Checks if the next DataFrame is available in the stream.
     * @return True if the next DataFrame is available, false otherwise.
     */
    bool next_data_frame_is_in_stream();

    /**
     * @brief Checks if the stream is almost finished based on the remaining time and the current time.
     * @param a_time_remaining The time remaining for the stream to be considered almost finished.
     * @param a_current_time The current time to compare with the end time of the stream.
     * @return True if the stream is almost finished, false otherwise.
     */
    bool is_almost_finished(uint64_t a_time_remaining, uint64_t a_current_time);

    /**
     * @brief Stops the data stream and cleans up resources.
     */
    void stop_stream();

    /**
     * @brief gets the time of the stream
     * @return TimeRange of stream
    */
    TimeRange* get_time_of_stream();

private:
    /**
     * @brief Updates the time range of the stream based on the data frames fetched.
     */
    void update_time();
private:
    DataStream* m_data_stream; ///< Pointer to the DataStream for fetching DataFrame objects.
    TimeRange* m_time_range_of_stream; ///< Time range for which DataFrames are fetched and managed.
    DataFrameRingbuffer* m_current_data_frames; ///< Ring buffer for storing and managing DataFrame objects.
};

#endif // DATA_FRAME_STREAM_H
