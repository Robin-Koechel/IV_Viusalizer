#include "InitialisationMessage.h"

InitialisationMessage::InitialisationMessage(Config& a_config, TimeRange& a_time_range) : m_config(a_config), m_time_range(a_time_range) {
}

Config& InitialisationMessage::get_config() {
    return this->m_config;
}

TimeRange& InitialisationMessage::get_time_range() {
    return this->m_time_range;
}

InitialisationMessage::~InitialisationMessage() {

}
