#include "ConfigContainer.h"

// Default constructor definition
ConfigContainer::ConfigContainer() : log_info(false), log_debug(false), jpg_image_quality(0) {}

ConfigContainer::ConfigContainer(bool logInfo, bool logDebug, int jpgQuality)
        : log_info(logInfo), log_debug(logDebug), jpg_image_quality(jpgQuality) {}

// Getter for log_info
bool ConfigContainer::getLogInfo() const {
    return log_info;
}

// Setter for log_info
void ConfigContainer::setLogInfo(bool value) {
    log_info = value;
}

// Getter for log_debug
bool ConfigContainer::getLogDebug() const {
    return log_debug;
}

// Setter for log_debug
void ConfigContainer::setLogDebug(bool value) {
    log_debug = value;
}

// Getter for jpg_image_quality
int ConfigContainer::getJpgImageQuality() const {
    return jpg_image_quality;
}

// Setter for jpg_image_quality
void ConfigContainer::setJpgImageQuality(int value) {
    jpg_image_quality = value;
}