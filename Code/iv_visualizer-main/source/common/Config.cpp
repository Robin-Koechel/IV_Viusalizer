#include "Config.h"

Config::Config(const ConfigContainer& container, const ConfigDetector& detector, const ConfigGStreamer& gstreamer)
    : containerConfig(container), detectorConfig(detector), gstreamerConfig(gstreamer) {
}

ConfigContainer Config::getContainerConfig() const {
    return containerConfig;
}

void Config::setContainerConfig(const ConfigContainer& value) {
    containerConfig = value;
}

ConfigDetector Config::getDetectorConfig() const {
    return detectorConfig;
}

void Config::setDetectorConfig(const ConfigDetector& value) {
    detectorConfig = value;
}

ConfigGStreamer Config::getGstreamerConfig() const {
    return gstreamerConfig;
}

void Config::setGstreamerConfig(const ConfigGStreamer& value) {
    gstreamerConfig = value;
}
