#include "ConfigContainer.h"

bool ConfigContainer::get_log_info() const
{
    return m_log_info;
}

void ConfigContainer::set_log_info(bool a_new_log_info)
{
    m_log_info = a_new_log_info;
}

bool ConfigContainer::get_log_debug() const
{
    return m_log_debug;
}

void ConfigContainer::set_log_debug(bool a_new_log_debug)
{
    m_log_debug = a_new_log_debug;
}

ConfigContainer::ConfigContainer() {
    m_jpg_image_quality = 0;
}

ConfigContainer::ConfigContainer(bool a_log_info, bool a_log_debug):
    m_log_info(a_log_info),
    m_log_debug(a_log_debug)
{}
int ConfigContainer::get_jpg_image_quality() const {
    return m_jpg_image_quality;
}

    
void ConfigContainer::set_jpg_image_quality(int a_jpg_image_quality){
    m_jpg_image_quality = a_jpg_image_quality;
}
