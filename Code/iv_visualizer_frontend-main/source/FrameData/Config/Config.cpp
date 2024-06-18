#include "Config.h"

Config::Config() {}

Config::Config(ConfigGStreamer* a_config_gstreamer, ConfigContainer* a_config_container, ConfigDetector* a_config_detector):
    m_config_gstreamer(*a_config_gstreamer),
    m_config_container(*a_config_container),
    m_config_detector(*a_config_detector)
{}

ConfigGStreamer& Config::get_config_gstreamer() {
    return m_config_gstreamer;
}

void Config::set_config_gstreamer(const ConfigGStreamer& a_new_config_gstreamer) {
    m_config_gstreamer = a_new_config_gstreamer;
}

ConfigContainer& Config::get_config_container() {
    return m_config_container;
}

void Config::set_config_container(const ConfigContainer& a_new_config_container) {
    m_config_container = a_new_config_container;
}

ConfigDetector& Config::get_config_detector() {
    return m_config_detector;
}

void Config::set_config_detector(const ConfigDetector& a_new_config_detector) {
    m_config_detector = a_new_config_detector;
}