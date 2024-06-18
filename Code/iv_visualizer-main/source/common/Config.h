#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigContainer.h"
#include "ConfigDetector.h"
#include "ConfigGStreamer.h"

class Config {
private:
    ConfigContainer containerConfig;
    ConfigDetector detectorConfig;
    ConfigGStreamer gstreamerConfig;

public:
    Config(const ConfigContainer& container, const ConfigDetector& detector, const ConfigGStreamer& gstreamer);

    // Getters and setters for each config component
    ConfigContainer getContainerConfig() const;
    void setContainerConfig(const ConfigContainer& value);

    ConfigDetector getDetectorConfig() const;
    void setDetectorConfig(const ConfigDetector& value);

    ConfigGStreamer getGstreamerConfig() const;
    void setGstreamerConfig(const ConfigGStreamer& value);
};

#endif // CONFIG_H
