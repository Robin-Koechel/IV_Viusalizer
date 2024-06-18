#ifndef GRPCREADER_H
#define GRPCREADER_H

#include "Reader.h"
#include "IvBackendClient.h"

/**
 * @class GRPCReader
 * @brief Class for reading data via gRPC protocol.
 */
class GRPCReader : public Reader {
public:
    /**
     * @brief Constructor for GRPCReader class.
     * @param url_grpc The gRPC URL.
     */
    GRPCReader(QString url_grpc);

    /**
     * @brief Read images from a specified URL stream within a time range and view type.
     * @param a_url_stream URL of the stream.
     * @param a_time_range Time range for reading images.
     * @param a_view_type Type of view.
     * @return DataStream pointer.
     */
    DataStream* read_images(const QString a_url_stream, TimeRange& a_time_range, const ViewTypeEnum& a_view_type) override;

    /**
     * @brief Read configuration from a specified URL stream.
     * @param a_url_stream URL of the stream.
     * @return Config pointer.
     */
    Config* read_config(const QString a_url_stream) override;

    /**
     * @brief Read live images from a specified URL stream with a given view type.
     * @param a_url_stream URL of the stream.
     * @param a_view_type Type of view.
     * @return DataStream pointer.
     */
    DataStream* read_live_images(const QString a_url_stream, const ViewTypeEnum& a_view_type) override;

    /**
     * @brief Read range information from the backend.
     * @return List of List of timeranges. First List TimeRange of Annotation and second list TimeRange of images.
     */
    QVector<QVector<TimeRange*>>* read_range_info(const QString urlStream);

    /**
     * @brief Read available URLs.
     * @return Vector of URLs.
     */
    QVector<QString>* read_available_urls();

private:
    IvBackendClient* m_backend; ///<source to aks
};

#endif // GRPCREADER_H
