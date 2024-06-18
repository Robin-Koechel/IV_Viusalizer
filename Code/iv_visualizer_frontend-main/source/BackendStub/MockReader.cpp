#include "MockReader.h"
#include "DataStreamMock.h"

MockReader::MockReader(QString a_url_grpc, TimeRange* a_time)
    : Reader(a_url_grpc), m_time(a_time), m_stream(nullptr) {
}

DataStream* MockReader::read_images(const QString a_url_stream, TimeRange& a_time_range, const ViewTypeEnum& a_view_type) {
    m_stream = new DataStreamMock(&a_time_range);
    connect(this, &MockReader::sig_set_config, m_stream, &DataStreamMock::slot_set_config);
    return m_stream;
}

DataStream* MockReader::read_live_images(const QString a_url_stream, const ViewTypeEnum& a_view_type) {
    return new DataStreamMock(m_time);
}


void MockReader::set_config(const Config& a_con) {
    m_current_config = a_con;
    emit sig_set_config(&m_current_config);
}

MockReader::~MockReader() {
    delete m_time;
    delete m_stream;
}

QVector<QVector<TimeRange*>>* MockReader::read_range_info(const QString urlStream){
   QVector<QVector<TimeRange*>>* time_info = new QVector<QVector<TimeRange*>>();
    time_info->append(QVector<TimeRange*>());
    time_info->append(QVector<TimeRange*>());
    (*time_info)[0].append(new TimeRange(*m_time)); 
    (*time_info)[1].append(new TimeRange(*m_time)); 

    return time_info;
}
Config* MockReader::read_config(const QString a_url_stream){
    ConfigGStreamer* gStreamer = new ConfigGStreamer(a_url_stream,0,0,0);
    gStreamer->set_stream_url(a_url_stream);
    Config* newCon  = new Config();
    newCon->set_config_gstreamer(*gStreamer);
    return newCon;
}