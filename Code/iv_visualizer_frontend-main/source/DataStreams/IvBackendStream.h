#ifndef IVBACKENDSTREAM_H
#define IVBACKENDSTREAM_H
#include "DataStream.h"
#include "../BackendConnector/IvBackendClient.h"
#include "ivbackend.grpc.pb.h"
#include <QMutex>
#include <QWaitCondition>
/**
 * @class IvBackendStream
 * @brief Implementation of DataStream for getting DataFrames from Backend.
 *
 * 
 */
class IvBackendStream : public DataStream
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a IvBackendStream
     * @param a_backend_client the backendClient where date gets produced
     */
    IvBackendStream(IvBackendClient* a_backend_client);

    /**
     * @brief Destructor for IvBackendStream, handles resource cleanup.
     */
    ~IvBackendStream();

    /**
     * @brief Pops the next DataFrame from the data queue.
     * @param a_create_data_frame flag to say if dataframe gets created
     * @return Pointer to the next DataFrame in the queue, or nullptr if the queue is empty.
     */
    DataFrame* pop(bool a_create_data_frame)override;

    /**
     * @brief Checks if all the data frames have been processed.
     * @return True if all data frames have been popped, false otherwise.
     */
    bool is_done()override;

    /**
     * @brief Signals to stop the data stream processing.
     */
    void stop_stream()override;

    /**
     * @brief Adds a DataFrame to the data queue.
     * @param data The data to add to the Stream.
     */
    void add_data_frame(const ivbackend::DataStream& data);
    
private:
    /**
     * @brief Generates a new DataFrame.
     * @param data The data to add to the Stream.
     */
    DataFrame* create_data_frame(const ivbackend::DataStream& data);

public:
    QVector<ivbackend::DataStream> m_data; ///< Queue of DataFrames to be processed.
    QMutex m_mutex; ///< Mutex for thread-safe access to shared resources.
    QWaitCondition m_condNotEmpty; ///< Signals when data queue is not empty.
    QWaitCondition m_condNotFull; ///< Signals when queue has space for more DataFrames.
    bool m_running; ///< Indicates whether the stream is running.
    int size = 10; ///< Size Of Buffer
    IvBackendClient* m_backend_client; ///< backendClient to stop stream
};

#endif // IVBACKENDSTREAM_H
