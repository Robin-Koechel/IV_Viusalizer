#ifndef MOCK_WRITER_H
#define MOCK_WRITER_H

#include "MockReader.h"
#include "../BackendConnector/Writer.h"
#include "../BackendConnector/TimeRange.h"
/**
 * @class MockWriter
 * @brief Mock implementation of the Writer interface for testing purposes.
 *
 * Provides a mock implementation of the Writer interface to facilitate
 * testing of components that require writing configurations, camera additions,
 * and database configurations without interacting with the actual backend services.
 */
class MockWriter : public Writer
{
public:
    /**
     * @brief Constructs a MockWriter object with specified gRPC URL, data range, and reader.
     * 
     * @param a_url_grpc The URL for the gRPC service.
     * @param a_range_of_data Pointer to the TimeRange object specifying the data range.
     * @param a_reader Pointer to the MockReader object for data reading operations.
     */
    MockWriter(QString a_url_grpc, TimeRange* a_range_of_data, MockReader* a_reader);

    /**
     * @brief Destructor for the MockWriter.
     */
    ~MockWriter();

    /**
     * @brief Writes configuration to a specified stream.
     * 
     * @param a_url_stream The URL of the stream where the configuration is to be written.
     * @param a_config_to_write The Config object containing the configuration to write.
     */
    void write_config(const QString a_url_stream, Config& a_config_to_write);

    /**
     * @brief Adds a camera to the system using the specified stream URL.
     * 
     * @param a_url_stream The URL of the stream for adding the camera.
     * @return true if connection is established
     */
    bool add_cam(const QString a_url_stream);


    /**
     * @brief stops the stream
     * @param url_stream the url to stop
    */
    void stop_cam(const QString url_stream) override;

private:
    TimeRange m_time; ///< Time range for data operations.
    MockReader* m_reader; ///< Pointer to the MockReader for associated reading operations.
    Config m_current_config; ///< Current configuration settings being used.
};

#endif // MOCK_WRITER_H

