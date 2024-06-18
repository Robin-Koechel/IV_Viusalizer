#ifndef DATA_FRAME_RING_BUFFER_H
#define DATA_FRAME_RING_BUFFER_H

#include "../FrameData/Data/DataFrame.h"
#include "../BackendConnector/TimeRange.h"
#include <QFutureWatcher>
#include <QObject>
#include <vector>

/**
 * @class DataFrameRingbuffer
 * @brief Manages a ring buffer for storing DataFrame objects.
 *
 * Implements a ring buffer to store DataFrame objects with a fixed capacity.
 * It provides functionality to add new frames, search for frames by position,
 * and manage the buffer's capacity and size. It supports asynchronous deletion
 * of frames to manage memory efficiently.
 */
class DataFrameRingbuffer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a DataFrameRingbuffer with the specified capacity.
     * @param a_capacity The maximum number of DataFrame objects the ring buffer can hold.
     */
    explicit DataFrameRingbuffer(int a_capacity);

    /**
     * @brief Destructor for DataFrameRingbuffer.
     */
    ~DataFrameRingbuffer();

    /**
     * @brief Gets the last added DataFrame object from the buffer.
     * @return A constant reference to the last added DataFrame.
     */
    const DataFrame& get_last_added_data_frame();

    /**
     * @brief Adds a new DataFrame object to the buffer.
     * @param a_to_insert Pointer to the DataFrame object to be added to the buffer.
     */
    void push(DataFrame* a_to_insert);

    /**
     * @brief Searches for a DataFrame in the buffer by its position as timestamp.
     * @param a_position The position to search for in the buffer.
     * @return A constant reference to the DataFrame at the specified position.
     */
    DataFrame* search_ring_buffer(uint64_t a_position);

    /**
     * @brief Checks if a DataFrame exists in the buffer at the specified position as timestamp.
     * @param a_position The position to check in the buffer.
     * @return True if the DataFrame exists at the specified position, false otherwise.
     */
    bool contains_data_frame(uint64_t a_position);

    /**
     * @brief Gets the start time from the first DataFrame in the buffer.
     * @return The start time as a uint64_t value.
     */
    uint64_t get_start_time();

    /**
     * @brief Gets the next DataFrame in the buffer.
     * @return A constant reference to the next DataFrame.
     */
    DataFrame* get_next_data_frame();

    /**
     * @brief Checks if the next DataFrame is already in the buffer.
     * @return True if the next DataFrame is in the buffer, false otherwise.
     */
    bool next_data_frame_is_in_buffer();

    /**
     * @brief Initiates asynchronous deletion of DataFrames from the buffer.
     */
    void delete_frames_async();

    /**
     * @brief Gets the current size of the buffer.
     * @return The current size as an integer.
     */
    int get_size();

    /**
     * @brief Gets the capacity of the buffer.
     * @return The capacity as an integer.
     */
    int get_capacity();


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

private:
    /**
     * @brief Deletes DataFrames from the buffer.
     */
    void delete_frames();
    /**
     * @brief Calculates (a_number_to_sub - 1) % a_modulo but positiv
     * @param a_number_to_sub the number to subtract
     * @param a_modulo the modulo number
     */
    int minus_one_modulo(int a_number_to_sub, int a_modulo);

private slots:
    /**
     * @brief Slot to clean up resources once the asynchronous deletion is complete.
     */
    void slot_cleanup();

private:
    std::vector<DataFrame*> m_ring_buffer; ///< The ring buffer storing DataFrame objects.
    TimeRange m_current_time; ///< The current time range of the DataFrames in the buffer.
    int m_capacity; ///< The maximum capacity of the ring buffer.
    int m_last_index; ///< The index of the last added DataFrame.
    int m_last_selected_index; ///< The index of the last selected DataFrame for retrieval.
    int m_start_index; ///< The index of the first DataFrame in the buffer.
    QFutureWatcher<void> m_future_watcher; ///< Watches the future of the asynchronous delete operation.
};

#endif // DATA_FRAME_RING_BUFFER_H
