#include "Stream.h"

#include "../BackendStub/MockReader.h"
#include "GrpcReader.h"
#include "GrpcWriter.h"
#include "../main/MainWindow.h"
#include <QSettings>

Stream::Stream(const QString a_url_stream, const QString a_url_grpc) {
    this->m_reader = new GRPCReader(a_url_grpc);
    this->m_writer = new GRPCWriter(a_url_grpc);
    m_connection_url = a_url_stream;
    m_connected = this->m_writer->add_cam(a_url_stream);
}

Stream::Stream(MockReader* a_mock, MockWriter* a_mock_writer) {
  this->m_writer = a_mock_writer;
  this->m_reader = a_mock;
  this->m_config = m_reader->read_config("a_url_stream");
  m_connection_url = "a_url_stream";
}

Stream::Stream(QString a_name, TimeRange* a_time_range, Config* a_config, Writer* a_writer, Reader* a_reader)
    : m_name(a_name),
      m_time_of_accessible_data(*a_time_range),
      m_config(a_config),
      m_writer(a_writer),
      m_reader(a_reader) {}

Stream::Stream() {}

void Stream::read_config_from_backend(){
  this->m_config = m_reader->read_config(m_connection_url);
}

DataFrameStream* Stream::get_image_stream(TimeRange& a_time_range,
                                          const ViewTypeEnum& a_view_type) {
  QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);
  DataStream* a_data_stream = this->m_reader->read_images(
      m_connection_url, a_time_range, a_view_type);
  uint64_t a_end_time = a_time_range.get_end_time();
  return new DataFrameStream(a_data_stream, a_end_time, settings.value(m_settings_stream_group + m_settings_frame_buffer).toInt());
}

DataFrameStream* Stream::get_live_stream(const ViewTypeEnum& a_view_type) {
    QSettings settings(MainWindow::m_config_file_path, QSettings::IniFormat);
  DataStream* a_data_stream = this->m_reader->read_live_images(
      m_connection_url, a_view_type);

  return new DataFrameStream(a_data_stream, std::numeric_limits<uint64_t>::max(), settings.value(m_settings_stream_group + m_settings_frame_buffer).toInt());
}


Config* Stream::get_config() { return this->m_config; }

Reader* Stream::get_reader() { return this->m_reader; }


Writer* Stream::get_writer() { return this->m_writer; }

TimeRange Stream::get_time_range_of_accessible_data(ViewTypeEnum a_view_type) {
  // Accessing the time range information
  QVector<QVector<TimeRange*>>* range_info = m_reader->read_range_info(m_connection_url);
  // Check if the retrieved data is valid before accessing it.
  if (range_info == nullptr || range_info->isEmpty()) {
    // Return a default TimeRange object or handle the error appropriately.
    return TimeRange(0,0); 
  }

  int index = 0;
  // Setting the index based on the view type.
  if (a_view_type == ViewTypeEnum::ONLY_VIDEO || a_view_type == ViewTypeEnum::ANNOTATION_AND_VIDEO) {
    index = 1;
  }

  // Ensuring the selected vector is not empty.
  if (range_info->at(index).isEmpty()) {
    // Return a default TimeRange object or handle the error appropriately.
    return TimeRange(0,0); 
  }    

  // Extracting the start time of the first and the end time of the last TimeRange objects in the selected vector.
  if (range_info == nullptr || range_info->isEmpty()) {
    return TimeRange();
  }

  uint64_t start_time = range_info->at(index).first()->get_start_time();
  uint64_t end_time = range_info->at(index).last()->get_end_time();

  for (int i = 0; i < range_info->size(); ++i) {
    QVector<TimeRange*>& inner_vector = (*range_info)[i];
    for (int j = 0; j < inner_vector.size(); ++j) {
        delete inner_vector[j];
    }
    inner_vector.clear();
  }
  delete range_info;

  return TimeRange(start_time, end_time);

}

void Stream::update_config() {
  this->m_writer->write_config(m_connection_url,
                               *this->m_config);
}

QString Stream::get_name() { return this->m_name; }

void Stream::set_name(QString a_new_name) { this->m_name = a_new_name; }

QString Stream::get_url() {
    return m_connection_url;
}


void Stream::stop_stream(){
    m_writer->stop_cam(m_connection_url);
}

bool Stream::is_connected(){
   return m_connected;
}

QVector<TimeRange*> Stream::get_lists_of_time_range_of_accessible_data(ViewTypeEnum a_view_type){
 
  // Accessing the time range information
  QVector<QVector<TimeRange*>>* range_info = m_reader->read_range_info(m_connection_url);
  // Check if the retrieved data is valid before accessing it.
  if (range_info == nullptr || range_info->isEmpty()) {
    // Return a default TimeRange object or handle the error appropriately.
    return QVector<TimeRange*>(); 
  }

  int index = 0;
  // Setting the index based on the view type.
  if (a_view_type == ViewTypeEnum::ONLY_VIDEO || a_view_type == ViewTypeEnum::ANNOTATION_AND_VIDEO) {
    index = 1;
  }

  // Ensuring the selected vector is not empty.
  if (range_info->at(index).isEmpty()) {
    // Return a default TimeRange object or handle the error appropriately.
    return QVector<TimeRange*>(); 
  }    

  // Extracting the start time of the first and the end time of the last TimeRange objects in the selected vector.
  if (range_info == nullptr || range_info->isEmpty()) {
    return QVector<TimeRange*>();
  }
  QVector<TimeRange*>& inner_vector = (*range_info)[(index + 1) % 2];
  for (int j = 0; j < inner_vector.size(); ++j) {
      delete inner_vector[j];
  }
  inner_vector.clear();
  return (*range_info)[index];
}
