#ifndef MOCK_READER_H
#define MOCK_READER_H

#include "../ViewType/ViewTypeEnum.h"
#include "../FrameData/Config/Config.h"
#include "../DataStreams/DataStream.h"
#include "../BackendConnector/Reader.h"
#include "../BackendConnector/TimeRange.h"
#include "DataStreamMock.h"

/**
 * @class MockReader
 * @brief Mock implementation of the Reader interface for testing purposes.
 *
 * This class provides a mock implementation of the Reader interface. It is designed to
 * support testing by simulating the reading of images, configurations, and database configurations
 * without requiring access to the actual backend services.
 */
class MockReader : public Reader
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a MockReader object with specified gRPC URL and time range.
     * @param a_url_grpc The URL for the gRPC service.
     * @param a_time Pointer to the TimeRange object specifying the time range for data retrieval.
     */
    MockReader(QString a_url_grpc, TimeRange* a_time);
    
    /**
     * @brief Destructor for the MockReader.
     */
    ~MockReader();

    /**
     * @brief Reads images from a specified stream within a given time range and view type.
     * @param a_url_stream URL of the stream to read images from.
     * @param a_time_range Time range for which images are to be read.
     * @param a_view_type Enum specifying the type of view required.
     * @return Pointer to a DataStream object containing the read images.
     */
    DataStream* read_images(const QString a_url_stream, TimeRange& a_time_range, const ViewTypeEnum& a_view_type) override;

    /**
     * @brief Reads configuration settings from a specified stream.
     * @param a_url_stream URL of the stream from which to read the configuration.
     * @return Pointer to a Config object containing the configuration settings.
     */
    Config* read_config(const QString a_url_stream) override;

    /**
     * @brief Reads live images from a specified stream with a given view type.
     * @param a_url_stream URL of the stream to read live images from.
     * @param a_view_type Enum specifying the type of view required for the live images.
     * @return Pointer to a DataStream object containing the live images.
     */
    DataStream* read_live_images(const QString a_url_stream, const ViewTypeEnum& a_view_type) override;

    /**
     * @brief Sets the configuration for the reader.
     * @param a_con The configuration settings to be applied.
     */
    void set_config(const Config& a_con);

    /**
     * @brief reads the range info from the backend
     *
     * @return 2 List of List of timeranges. First List TimeRange of Annotation and second list TimeRange of images.
     */
    QVector<QVector<TimeRange*>>* read_range_info(const QString urlStream);

signals:
    /**
     * @brief Signal to set configuration settings for the reader.
     * @param a_con Pointer to the configuration settings to be applied.
     */
    void sig_set_config(Config* a_con);

private:
    TimeRange* m_time; ///< Time range for data retrieval.
    Config m_current_config; ///< Current configuration settings.
    DataStreamMock* m_stream; ///< Mock data stream for image retrieval.
};

#endif // MOCK_READER_H
