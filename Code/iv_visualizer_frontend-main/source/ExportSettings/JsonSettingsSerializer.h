#ifndef JSONSETTINGSSERIALIZER_H
#define JSONSETTINGSSERIALIZER_H

#include <QJsonArray>
#include "../BackendConnector/Stream.h"

/**
 * @brief The JsonSettingsSerializer class handles serialization and deserialization of stream settings to/from JSON.
 */
class JsonSettingsSerializer {

public:
    /**
     * @brief Constructs a JsonSettingsSerializer object.
     */
    JsonSettingsSerializer();

    /**
     * @brief Destroys the JsonSettingsSerializer object.
     */
    ~JsonSettingsSerializer();

    /**
     * @brief Exports the provided streams to a JSON file.
     * @param a_streams The vector of streams to export.
     * @param a_file_name The name of the JSON file to export to.
     */
    void export_streams(QVector<Stream*> a_streams, QString a_file_name);

    /**
     * @brief Imports streams from a JSON file.
     * @param a_file_name The name of the JSON file to import from.
     * @return A vector of streams imported from the JSON file.
     */
    QVector<Stream*> import_streams(QString a_file_name, bool a_is_mock);

private:
    /**
     * @brief Serializes a vector of streams to a JSON array.
     * @param a_streams The vector of streams to serialize.
     * @return The serialized JSON array.
     */
    QJsonArray serialize_streams(QVector<Stream*> a_streams);

    /**
     * @brief Serializes a vector of regions of interest to a JSON array.
     * @param a_rois The vector of regions of interest to serialize.
     * @return The serialized JSON array.
     */
    QJsonArray serialize_streams_region_of_interest(const QVector<RegionOfInterest*> a_rois);

    /**
     * @brief Writes a JSON array to a file.
     * @param a_json_array The JSON array to write.
     * @param a_file_name The name of the file to write to.
     */
    void to_json_file(QJsonArray a_json_array, QString a_file_name);

    /**
     * @brief Reads stream settings from a JSON file.
     * @param a_file_name The name of the JSON file to read from.
     * @return A vector of streams read from the JSON file.
     */
    QVector<Stream*> read_stream_json_file(QString a_file_name,  bool a_is_mock);

    /**
     * @brief Deserializes stream attributes from a JSON array.
     * @param a_json_array The JSON array containing stream attributes.
     * @return A vector of streams with deserialized attributes.
     */
    QVector<Stream*> deserialize_stream_attributes(QJsonArray a_json_array, bool a_is_mock);

    /**
     * @brief Deserializes regions of interest from a JSON array.
     * @param roisArray The JSON array containing regions of interest.
     * @return A vector of regions of interest.
     */
    QVector<RegionOfInterest*> deserialize_streams_region_of_interest(QJsonArray a_rois_array);

private:
/**
 * @brief Error message for no streams that could be exported - no export file created.
 */
const QString m_msg_error_streams_not_exported =  "no streams that could be exported - no export file created"; /**< No streams that could be exported - no export file created. */

/**
 * @brief Error message for an error while gathering stream information.
 */
const QString m_msg_error_gathering_info = "error while gathering stream information"; /**< Error while gathering stream information. */

/**
 * @brief Success message for successfully written to file.
 */
const QString m_msg_error_write_success = "successfully written to file"; /**< Successfully written to file. */

/**
 * @brief Error message for failed to write into file.
 */
const QString m_msg_error_write_fail = "failed to write into file"; /**< Failed to write into file. */

/**
 * @brief Error message for failed to open file.
 */
const QString m_msg_error_open_fail = "failed open file"; /**< Failed to open file. */

/**
 * @brief Error message for invalid JSON format.
 */
const QString m_msg_error_json_invalid = "invalid json Format"; /**< Invalid JSON format. */

/**
 * @brief Constant for stream name.
 */
const QString m_stream_name = "stream_name"; /**< Stream name. */

/**
 * @brief Constant for stream reader URL.
 */
const QString m_stream_reader_url = "stream_reader_url"; /**< Stream reader URL. */

/**
 * @brief Constant for stream writer URL.
 */
const QString m_stream_writer_url = "stream_writer_url"; /**< Stream writer URL. */

/**
 * @brief Constant for stream URL.
 */
const QString m_stream_url = "stream_url"; /**< Stream URL. */

/**
 * @brief Constant for stream target FPS.
 */
const QString m_stream_target_fps = "stream_target_fps"; /**< Stream target FPS. */

/**
 * @brief Constant for stream custom size width.
 */
const QString m_stream_custom_size_width = "stream_custom_size_width"; /**< Stream custom size width. */

/**
 * @brief Constant for stream custom size height.
 */
const QString m_stream_custom_size_height = "stream_custom_size_height"; /**< Stream custom size height. */

/**
 * @brief Constant for stream log info.
 */
const QString m_stream_log_info = "stream_log_info"; /**< Stream log info. */

/**
 * @brief Constant for stream log debug.
 */
const QString m_stream_log_debug = "stream_log_debug"; /**< Stream log debug. */

/**
 * @brief Constant for stream CUDA device.
 */
const QString m_stream_cuda_device = "stream_cuda_device"; /**< Stream CUDA device. */

/**
 * @brief Constant for stream NMS threshold.
 */
const QString m_stream_nms_threshold = "stream_nms_threshold"; /**< Stream NMS threshold. */

/**
 * @brief Constant for stream confidence threshold.
 */
const QString m_stream_confidence_threshold = "stream_confidence_threshold"; /**< Stream confidence threshold. */

/**
 * @brief Constant for stream inference type.
 */
const QString m_stream_inference_type = "stream_inference_type"; /**< Stream inference type. */

/**
 * @brief Constant for stream regions of interest.
 */
const QString m_stream_regions_of_interest = "stream_regions_of_interest"; /**< Stream regions of interest. */

/**
 * @brief Constant for region of interest name.
 */
const QString m_roi_name = "roi_name"; /**< Region of interest name. */

/**
 * @brief Constant for region of interest ID.
 */
const QString m_roi_id = "roi_id"; /**< Region of interest ID. */

/**
 * @brief Constant for region of interest inside flag.
 */
const QString m_roi_is_inside = "roi_is_inside"; /**< Region of interest inside flag. */

/**
 * @brief Constant for region of interest color.
 */
const QString m_roi_color = "roi_color"; /**< Region of interest color. */

/**
 * @brief Constant for region of interest opacity.
 */
const QString m_roi_opacity = "roi_opacity"; /**< Region of interest opacity. */

/**
 * @brief Constant for region of interest point X coordinate.
 */
const QString m_roi_point_x = "point_x"; /**< Region of interest point X coordinate. */

/**
 * @brief Constant for region of interest point Y coordinate.
 */
const QString m_roi_point_y = "point_y"; /**< Region of interest point Y coordinate. */

/**
 * @brief Constant for region of interest polygon.
 */
const QString m_roi_polygon = "roi_polygon"; /**< Region of interest polygon. */
};

#endif // JSONSETTINGSSERIALIZER_H
