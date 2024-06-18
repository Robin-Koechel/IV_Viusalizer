#ifndef CONFIG_H
#define CONFIG_H

#include "ConfigGStreamer.h"
#include "ConfigDetector.h"
#include "ConfigContainer.h"

/**
 * @class Config
 * @brief Represents a configuration holder for various subsystems.
 * 
 * This class encapsulates the configurations for different subsystems,
 * including GStreamer, the detection configuration, and other general
 * container configurations. Provides access and modification methods for each.
 */
class Config
{
private:
    ConfigGStreamer m_config_gstreamer; ///< Configuration for GStreamer.
    ConfigContainer m_config_container; ///< General container configuration.
    ConfigDetector m_config_detector;   ///< Detection system configuration.

public:
    /**
     * @brief Constructs a Config object with default settings.
     */
    Config();

    /**
     * @brief Constructs a Config object with defined attributes
     * 
     * @param a_config_gstreamer The new GStreamer configuration to set.
     * @param a_config_container The new container configuration to set.
     * @param a_config_detector The new detection configuration to set.
    */
    Config(ConfigGStreamer* a_config_gstreamer, ConfigContainer* a_config_container, ConfigDetector* a_config_detector);

    // copy 
    Config& operator=(const Config& other) {
        if (this != &other) {
            // Copy members from 'other' to 'this'
            m_config_gstreamer = other.m_config_gstreamer;
            m_config_container = other.m_config_container;
            m_config_detector = other.m_config_detector;
        }
        return *this;
    }

    /**
     * @brief Gets the current GStreamer configuration.
     * @return A reference to the current GStreamer configuration.
     */
    ConfigGStreamer& get_config_gstreamer();

    /**
     * @brief Sets a new GStreamer configuration.
     * @param a_new_config_gstreamer The new GStreamer configuration to set.
     */
    void set_config_gstreamer(const ConfigGStreamer& a_new_config_gstreamer);

    /**
     * @brief Gets the current container configuration.
     * @return A reference to the current container configuration.
     */
    ConfigContainer& get_config_container();

    /**
     * @brief Sets a new container configuration.
     * @param a_new_config_container The new container configuration to set.
     */
    void set_config_container(const ConfigContainer& a_new_config_container);

    /**
     * @brief Gets the current detection configuration.
     * @return A reference to the current detection configuration.
     */
    ConfigDetector& get_config_detector();

    /**
     * @brief Sets a new detection configuration.
     * @param a_new_config_detector The new detection configuration to set.
     */
    void set_config_detector(const ConfigDetector& a_new_config_detector);
};

#endif // CONFIG_H
