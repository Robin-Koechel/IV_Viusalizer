#ifndef DATASTREAM_H
#define DATASTREAM_H

#include "../FrameData/Data/DataFrame.h"
#include <QObject>

/**
 * @class DataStream
 * @brief Abstract class representing a stream of DataFrame objects.
 *
 * DataStream defines an interface for classes that represent a stream of
 * DataFrame objects. It is designed to be subclassed by specific implementations
 * that provide the actual data fetching mechanism.
 */
class DataStream : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a DataStream object.
     */
    DataStream();

    /**
     * @brief Retrieves the next DataFrame from the stream.
     * 
     * Pure virtual function that must be implemented by subclasses to return
     * the next available DataFrame in the stream. If no more DataFrames are
     * available, it should return nullptr.
     * @param a_create_data_frame flag to say if dataframe gets created
     * @return Pointer to the next DataFrame object, or nullptr if the stream is exhausted.
     */
    virtual DataFrame* pop(bool a_create_data_frame) = 0;

    /**
     * @brief Checks whether the data stream has been exhausted.
     * 
     * Pure virtual function that must be implemented by subclasses to indicate
     * whether all DataFrames have been retrieved from the stream.
     * 
     * @return True if no more DataFrames are available, false otherwise.
     */
    virtual bool is_done() = 0;

    /**
     * @brief Stops the data stream.
     * 
     * Pure virtual function that must be implemented by subclasses to perform
     * any necessary cleanup operations to stop the data stream gracefully.
     */
    virtual void stop_stream() = 0;
};

#endif // DATASTREAM_H
