#include "JsonSettingsSerializer.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <iostream>
#include <QSettings>
#include "../FrameData/Config/InferenceType.h"
#include "../main/MainWindow.h"

JsonSettingsSerializer::JsonSettingsSerializer() {}

JsonSettingsSerializer::~JsonSettingsSerializer() {}

void JsonSettingsSerializer::export_streams(QVector<Stream*> a_streams, QString a_file_name) {
  if (!a_streams.empty()) {
    to_json_file(serialize_streams(a_streams), a_file_name);
  } else {
    QMessageBox a_msg_box;
    a_msg_box.setText(m_msg_error_streams_not_exported);
    a_msg_box.exec();
  }
}

QVector<Stream*> JsonSettingsSerializer::import_streams(QString a_file_name, bool a_is_mock) {
  return read_stream_json_file(a_file_name, a_is_mock);
}

QJsonArray JsonSettingsSerializer::serialize_streams(QVector<Stream*> a_streams) {
  QJsonArray a_json_streams;
  for (Stream* a_stream : a_streams) {
    QJsonObject a_json_stream;
    try {
      a_json_stream[m_stream_name] = a_stream->get_name();
      a_json_stream[m_stream_reader_url] = a_stream->get_reader()->get_url();
      a_json_stream[m_stream_writer_url] = a_stream->get_writer()->get_url();
      // config
      a_json_stream[m_stream_url] =
          a_stream->get_config()->get_config_gstreamer().get_stream_url();
      a_json_stream[m_stream_target_fps] =
          a_stream->get_config()->get_config_gstreamer().get_target_fps();
      a_json_stream[m_stream_custom_size_width] =
          a_stream->get_config()->get_config_gstreamer().get_custom_size_width();
      a_json_stream[m_stream_custom_size_height] =
          a_stream->get_config()->get_config_gstreamer().get_custom_size_height();
      a_json_stream[m_stream_log_info] =
          a_stream->get_config()->get_config_container().get_log_info();
      a_json_stream[m_stream_log_debug] =
          a_stream->get_config()->get_config_container().get_log_debug();
      a_json_stream[m_stream_cuda_device] =
          a_stream->get_config()->get_config_detector().get_cuda_device();
      a_json_stream[m_stream_nms_threshold] =
          a_stream->get_config()->get_config_detector().get_nms_threshold();
      a_json_stream[m_stream_confidence_threshold] =
          a_stream->get_config()->get_config_detector().get_confidence_threshold();
      a_json_stream[m_stream_inference_type] =
          a_stream->get_config()->get_config_detector().get_inference_type();

      QJsonArray a_regions_of_interest = serialize_streams_region_of_interest(
          a_stream->get_config()->get_config_detector().get_rois());
      a_json_stream.insert(m_stream_regions_of_interest, a_regions_of_interest);
      a_json_streams.append(a_json_stream);
    } catch (...) {
      QMessageBox a_msg_box;
      a_msg_box.setText(m_msg_error_gathering_info);
      a_msg_box.exec();
    }
  }
  return a_json_streams;
}

QJsonArray JsonSettingsSerializer::serialize_streams_region_of_interest(
    const QVector<RegionOfInterest*> a_rois) {
  QJsonArray a_regions_of_interest;
  for (RegionOfInterest* a_roi : a_rois) {
    QJsonObject a_region_of_interest;

    try {
      a_region_of_interest[m_roi_name] = a_roi->get_name();
      a_region_of_interest[m_roi_id] = a_roi->get_id();
      a_region_of_interest[m_roi_is_inside] = a_roi->is_inside();
      a_region_of_interest[m_roi_color] = a_roi->get_color().name();
      a_region_of_interest[m_roi_opacity] = a_roi->get_opacity();

      QJsonArray a_polygon;
      for (int i = 0; i < a_roi->get_polygon().size(); i++) {
        QJsonObject a_json_point;
        QPoint a_point = a_roi->get_polygon().at(i);
        a_json_point[m_roi_point_x] = a_point.x();
        a_json_point[m_roi_point_y] = a_point.y();
        a_polygon.append(a_json_point);
      }
      a_region_of_interest.insert(m_roi_polygon, a_polygon);
      a_regions_of_interest.append(a_region_of_interest);
    } catch (...) {
      QMessageBox a_msg_box;
      a_msg_box.setText(m_msg_error_gathering_info);
      a_msg_box.exec();
    }
  }

  return a_regions_of_interest;
}

void JsonSettingsSerializer::to_json_file(QJsonArray a_json_array, QString a_file_name) {
  QJsonDocument a_json_doc(a_json_array);
  QByteArray a_json_data = a_json_doc.toJson();

  // Write JSON data to a file
  QFile a_file(a_file_name);
  if (a_file.open(QIODevice::WriteOnly)) {
    a_file.write(a_json_data);
    a_file.close();
    QMessageBox a_msg_box;
    a_msg_box.setText(m_msg_error_write_success);
    a_msg_box.exec();
  } else {
    QMessageBox a_msg_box;
    a_msg_box.setText(m_msg_error_write_fail);
    a_msg_box.exec();
  }
}

QVector<Stream*> JsonSettingsSerializer::read_stream_json_file(QString a_file_name,
                                                               bool a_is_mock) {
  QFile a_file(a_file_name);
  // Read JSON data from file
  if (a_file.open(QIODevice::ReadOnly)) {
    QByteArray a_file_data = a_file.readAll();
    a_file.close();

    // Deserialize JSON data to objects
    QJsonDocument a_doc = QJsonDocument::fromJson(a_file_data);
    QJsonArray a_json_array_read = a_doc.array();
    QVector<Stream*> a_objects_read =
        deserialize_stream_attributes(a_json_array_read, a_is_mock);
    return a_objects_read;
  } else {
    QMessageBox a_msg_box;
    a_msg_box.setText(m_msg_error_open_fail);
    a_msg_box.exec();
  }
  QVector<Stream*> a_objects_read = {};
  return a_objects_read;
}


QVector<Stream*> JsonSettingsSerializer::deserialize_stream_attributes(QJsonArray a_json_array,
                                                                       bool a_is_mock) {
  QVector<Stream*> a_streams;

  for (const auto& a_json_stream : a_json_array) {
    if (!a_json_stream.isObject()) {
      QMessageBox a_msg_box;
      a_msg_box.setText(m_msg_error_json_invalid);
      a_msg_box.exec();
    } else {

      const QJsonObject& a_stream_object = a_json_stream.toObject();


      // Deserialize config
      QString a_stream_url = a_stream_object[m_stream_name].toString();
      int a_target_fps = a_stream_object[m_stream_target_fps].toInt();
      int a_custom_size_width = a_stream_object[m_stream_custom_size_width].toInt();
      int a_custom_size_height = a_stream_object[m_stream_custom_size_height].toInt();
      ConfigGStreamer* gstreamer_conf = new ConfigGStreamer(
          a_stream_url, a_target_fps, a_custom_size_width, a_custom_size_height);

      bool a_log_info = a_stream_object[m_stream_log_info].toBool();
      bool a_log_debug = a_stream_object[m_stream_log_debug].toBool();
      ConfigContainer* container_config = new ConfigContainer(a_log_info, a_log_debug);

      int a_cuda_device = a_stream_object[m_stream_cuda_device].toInt();
      double a_nms_threshold = a_stream_object[m_stream_nms_threshold].toDouble();
      int a_confidence_threshold =
          a_stream_object[m_stream_confidence_threshold].toInt();
      InferenceType a_inference_type =
          static_cast<InferenceType>(a_stream_object[m_stream_inference_type].toInt());  // Since enums elements are int's the allocation should be fine

      QJsonArray a_rois_array = a_stream_object[m_stream_regions_of_interest].toArray();
      QVector<RegionOfInterest*> m_rois =
          deserialize_streams_region_of_interest(a_rois_array);
      ConfigDetector* a_detector_config = new ConfigDetector(
          a_cuda_device, a_nms_threshold, a_confidence_threshold, a_inference_type, m_rois);

      if (a_is_mock) {
        uint64_t a_number_of_frames = 18000;
        TimeRange* a_time = new TimeRange(0, a_number_of_frames * 1000000000);
        MockReader* a_mockreader =
            new MockReader(a_stream_object[m_stream_reader_url].toString(), a_time);
        MockWriter* a_mockwriter = new MockWriter(
            a_stream_object[m_stream_writer_url].toString(), a_time, a_mockreader);
        Stream* a_stream_to_add = new Stream(a_mockreader, a_mockwriter);
        a_stream_to_add->set_name(a_stream_object[m_stream_name].toString());

        Config* a_tmp_config = a_stream_to_add->get_config();
        a_tmp_config->set_config_container(*container_config);
        a_tmp_config->set_config_detector(*a_detector_config);
        a_tmp_config->set_config_gstreamer(*gstreamer_conf);
        a_stream_to_add->update_config();

        a_streams.append(a_stream_to_add);
      } else {
        QString a_name = a_stream_object[m_stream_name].toString();
        QSettings a_settings(MainWindow::m_config_file_path, QSettings::IniFormat);
        QString a_grpc_url = a_settings.value(MainWindow::m_settings_group_grpc + MainWindow::m_settings_grpc_url).toString();     
        Stream* a_stream = new Stream(a_stream_url, a_grpc_url);
        a_stream->set_name(a_name);
        a_streams.append(a_stream);
      }
    }
  }

  return a_streams;
}



QVector<RegionOfInterest*> JsonSettingsSerializer::deserialize_streams_region_of_interest(QJsonArray a_rois_array) {
  QVector<RegionOfInterest*> a_rois;

  for (const auto& json_roi : a_rois_array) {
    if (!json_roi.isObject()) {
      QMessageBox a_msg_box;
      a_msg_box.setText(m_msg_error_json_invalid);
      a_msg_box.exec();
    } else {

      const QJsonObject& a_roi_object = json_roi.toObject();


      // Deserialize ROI attributes
      QString a_name = a_roi_object[m_roi_name].toString();
      int a_id = a_roi_object[m_roi_id].toInt();
      bool a_is_inside = a_roi_object[m_roi_is_inside].toBool();

      // Deserialize color
      QString a_color_name = a_roi_object[m_roi_color].toString();
      QColor a_color(a_color_name);


      // Deserialize opacity
      float a_opacity = a_roi_object[m_roi_opacity].toDouble();


      // Deserialize polygon
      QJsonArray a_polygon_array = a_roi_object[m_roi_polygon].toArray();
      QPolygon* a_polygon = new QPolygon();

      for (const auto& a_json_point : a_polygon_array) {
        if (!a_json_point.isObject()) {
          // Handle error: invalid JSON format for polygon point
          continue;
        }

        const QJsonObject& a_point_object = a_json_point.toObject();
        int a_point_x = a_point_object[m_roi_point_x].toInt();
        int a_point_y = a_point_object[m_roi_point_y].toInt();
        a_polygon->append(QPoint(a_point_x, a_point_y));
      }
      RegionOfInterest* a_roi =
          new RegionOfInterest(a_name, a_id, *a_polygon, a_is_inside, a_opacity);

      // Add the deserialized ROI to the vector
      a_rois.append(a_roi);
    }
  }

  return a_rois;
}