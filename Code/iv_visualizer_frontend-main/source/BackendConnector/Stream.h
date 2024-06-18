/**
 * @file stream.h
 * @brief Contains the definition of the Stream class.
 */

#ifndef STREAM_H
#define STREAM_H

#include "GrpcReader.h"
#include "GrpcWriter.h"
#include "../BackendStub/MockReader.h"
#include "../BackendStub/MockWriter.h"
#include "../DataStreams/DataFrameStream.h"

/**
 * @brief Class for managing streams.
 */
class Stream {
private:
    const QString m_settings_stream_group = "Stream/";
    const QString m_settings_frame_buffer = "NumberOfBufferdFrames";
    Reader* m_reader; /**< Pointer to a Reader object to read Information from Backend. */
    Writer* m_writer; /**< Pointer to a Writer object to write on to the Backend. */
    Config* m_config; /**< Configuration object. */
    TimeRange m_time_of_accessible_data; /**< Time range of accessible data. */
    QString m_name; /**< Name of the stream. */
    QString m_connection_url; /**<Url to connect to the backend*/
    bool m_connected; /**<Flag to sa if the stream is connected to the processing tools*/


public:
    /**
     * @brief Constructor for Stream.
     * @param a_url_stream The URL of the stream.
     * @param a_url_grpc The URL for gRPC communication.
     */
    Stream(const QString a_url_stream, const QString a_url_grpc);

    /**
     * @brief Constructor for Stream with mock objects.
     * @param a_mock Pointer to a MockReader object.
     * @param a_mock_writer Pointer to a MockWriter object.
     */
    Stream(MockReader* a_mock, MockWriter* a_mock_writer);

    /**
     * @brief Constructs a Stream object with the specified attributes.
     *
     * This constructor initializes a Stream object with the given name, time range,
     * configuration, writer, and reader.
     *
     * @param a_name The name of the stream.
     * @param a_time_range The time range of accessible data for the stream.
     * @param a_config The configuration settings for the stream.
     * @param m_writer Pointer to the writer associated with the stream.
     * @param m_reader Pointer to the reader associated with the stream.
     */
    Stream(QString a_name, TimeRange* a_time_range, Config* a_config, Writer* m_writer, Reader* m_reader);

    /**
     * @brief Default constructor for Stream.
     */
    Stream();

    /**
     * @brief Read Config from Backend.
     */
    void read_config_from_backend();

    /**
     * @brief tells if the stream is connected to the processing tools
     * @return true if it is else false
    */
    bool is_connected();

    /**
     * @brief Gets the image stream.
     * @param a_time_range The time range.
     * @param a_view_type The view type.
     * @return Pointer to a DataFrameStream.
     */
    DataFrameStream* get_image_stream(TimeRange& a_time_range, const ViewTypeEnum& a_view_type);

    /**
     * @brief Gets the live stream.
     * @param a_view_type The view type.
     * @return Pointer to a DataFrameStream.
     */
    DataFrameStream* get_live_stream(const ViewTypeEnum& a_view_type);

    /**
     * @brief Gets the configuration.
     * @return Reference to the configuration.
     */
    Config* get_config();

    /**
     * @brief Gets the time range of accessible data.
     * @param a_view_type the view type enum to look up
     * @return Reference to the time range of accessible data.
     */
    TimeRange get_time_range_of_accessible_data(ViewTypeEnum a_view_type);
     
    /**
    * @brief Gets the reader.
    * @return Reference to the reader.
    */
    Reader* get_reader();

    /**
    * @brief Gets the writer.
    * @return Reference to the writer.
    */
    Writer* get_writer();

    /**
     * @brief Updates the configuration.
     */
    void update_config();

    /**
     * @brief Gets the name of the stream.
     * @return The name of the stream.
     */
    QString get_name();

    /**
     * @brief Sets the name of the stream.
     * @param a_new_name The new name of the stream.
     */
    void set_name(QString a_new_name);

    /**
     * @brief Gets the URL of the stream.
     * @return The URL of the stream.
     */
    QString get_url();
    /**
     * @brief stops the Streams
    */
    void stop_stream();

    /**
     * @brief Gets the whole list of TimeRanges
     * @param a_view_type to decide which timeranges to pick.
     */
    QVector<TimeRange*> get_lists_of_time_range_of_accessible_data(ViewTypeEnum a_view_type);

};

#endif // STREAM_H
