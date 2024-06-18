#include "MockWriter.h"

MockWriter::MockWriter(QString a_url_grpc, TimeRange* a_range_of_data, MockReader* a_reader) 
    : Writer(a_url_grpc), m_time(*a_range_of_data), m_reader(a_reader) {
}

void MockWriter::write_config(const QString a_url_stream, Config& a_config_to_write) {
    m_reader->set_config(a_config_to_write);
    m_current_config = a_config_to_write;
}

bool MockWriter::add_cam(const QString a_url_stream) {
    ConfigGStreamer gStreamer;
    gStreamer.set_stream_url(a_url_stream); 
    Config new_config;
    new_config.set_config_gstreamer(gStreamer);
    new_config.set_config_detector(m_current_config.get_config_detector()); 
    write_config(a_url_stream, new_config);
    InitialisationMessage* ini = new InitialisationMessage(new_config, m_time);
    return true;
}



MockWriter::~MockWriter(){

}
void MockWriter::stop_cam(const QString url_stream){
    
}
