#ifndef DATA_STREAM_MOCK_H
#define DATA_STREAM_MOCK_H

#include "../DataStreams/DataFrameStream.h"
#include "../FrameData/Data/DataFrame.h"
#include "../BackendConnector/TimeRange.h"
#include "../FrameData/Config/Config.h"
#include "../FrameData/Data/BoundingBox.h"

/**
 * @class DataStreamMock
 * @brief Mock implementation of DataStream for testing and simulation purposes.
 *
 * This class serves as a mock version of the DataStream interface, intended for use in testing
 * and simulation scenarios. It allows for the emulation of data stream behavior without the need
 * for a live data source.
 */
class DataStreamMock : public DataStream
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a DataStreamMock object with a specified time range.
     * @param a_time_range Pointer to the TimeRange object specifying the time range for the mock data.
     */
    DataStreamMock(TimeRange *a_time_range);

    /**
     * @brief Destructor for DataStreamMock, handles resource cleanup.
     */
    ~DataStreamMock();

    /**
     * @brief Pops the next DataFrame from the data queue.
     * @param a_create_data_frame flag to say if dataframe gets created
     * @return Pointer to the next DataFrame in the queue, or nullptr if the queue is empty.
     */
    DataFrame* pop(bool a_create_data_frame);

    /**
     * @brief Checks if all the data frames have been processed.
     * @return True if all data frames have been popped, false otherwise.
     */
    bool is_done();

    /**
     * @brief Signals to stop the data stream processing.
     */
    void stop_stream();

public slots:
    /**
     * @brief Sets the configuration for the data stream.
     * @param a_config Pointer to the Config object to apply to the data stream.
     */
    void slot_set_config(Config* a_config);

private:
    /**
     * @brief Adds a DataFrame to the data queue.
     * @param a_timestamp The timestamp for the new DataFrame.
     * @param a_annotations Vector of pointers to BoundingBox objects for annotations.
     * @param a_regions Vector of pointers to RegionOfInterest objects.
     */
    void add_data_frame(uint64_t a_timestamp, QVector<BoundingBox *> a_annotations, QVector<RegionOfInterest*> a_regions);

    /**
     * @brief Generates a new DataFrame and adds it to the queue.
     */
    void create_data_frame();

public:
    QVector<DataFrame*> m_data; ///< Queue of DataFrames to be processed.
    uint64_t m_asked_data_frame = 0; ///< Counter for the number of DataFrames requested.
    TimeRange* m_time_range; ///< Time range for generating mock DataFrames.
    Config* m_config; ///< Configuration settings for the data stream.
    QMutex m_mutex_config; ///< Mutex for thread-safe configuration access.
};

#endif // DATA_STREAM_MOCK_H
