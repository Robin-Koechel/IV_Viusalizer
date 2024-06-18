#include "ConfigGStreamer.h"

ConfigGStreamer::ConfigGStreamer()
    : m_stream_url(""), m_target_fps(m_default_target_fps), m_custom_size_width(m_default_size_width), m_custom_size_height(m_default_size_height) {
        m_stream_url = "";
        m_target_fps = m_default_target_fps;
        m_custom_size_width = m_default_size_width;
        m_custom_size_height = m_default_size_height;
    }

ConfigGStreamer::ConfigGStreamer(QString a_stream_url, int a_target_fps, int a_custom_size_width, int a_custom_size_height):
m_stream_url(a_stream_url),
m_target_fps(a_target_fps),
m_custom_size_width(a_custom_size_width),
m_custom_size_height(a_custom_size_height)
{}

QString ConfigGStreamer::get_stream_url() const {
    return m_stream_url;
}

void ConfigGStreamer::set_stream_url(const QString& a_new_stream_url) {
    m_stream_url = a_new_stream_url;
}

int ConfigGStreamer::get_target_fps() const {
    return m_target_fps;
}

void ConfigGStreamer::set_target_fps(int a_new_target_fps) {
    m_target_fps = a_new_target_fps;
}

int ConfigGStreamer::get_custom_size_width() const {
    return m_custom_size_width;
}

void ConfigGStreamer::set_custom_size_width(int a_new_custom_size_width) {
    m_custom_size_width = a_new_custom_size_width;
}

int ConfigGStreamer::get_custom_size_height() const {
    return m_custom_size_height;
}

void ConfigGStreamer::set_custom_size_height(int a_new_custom_size_height) {
    m_custom_size_height = a_new_custom_size_height;
}
