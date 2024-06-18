#ifndef CONFIG_CONTAINER_H
#define CONFIG_CONTAINER_H

/**
 * @class ConfigContainer
 * @brief Holds configuration settings related to logging.
 *
 * This class encapsulates configuration settings for logging purposes,
 * including flags to enable or disable information and debug logging.
 */
class ConfigContainer
{

public:
    /**
     * @brief Constructs a ConfigContainer with default logging settings.
     */
    ConfigContainer();

    /**
     * @brief Constructs a ConfigContainer object with the specified logging settings.
     *
     * This constructor initializes a ConfigContainer object with the given logging
     * settings for information and debug messages.
     *
     * @param a_log_info Flag indicating whether information logging is enabled.
     * @param a_log_debug Flag indicating whether debug logging is enabled.
     */
    ConfigContainer(bool a_log_info, bool a_log_debug);

    // Assignment operator
    ConfigContainer& operator=(const ConfigContainer& other) {
        if (this != &other) {
            // Copy members from 'other' to 'this'
            m_log_info = other.m_log_info;
            m_log_debug = other.m_log_debug;
            m_jpg_image_quality = other.m_jpg_image_quality;
        }
        return *this;
    }

    /**
     * @brief Retrieves the current info logging flag.
     * @return True if info logging is enabled, false otherwise.
     */
    bool get_log_info() const;

    /**
     * @brief Sets the info logging flag.
     * @param a_new_log_info The new value for the info logging flag.
     */
    void set_log_info(bool a_new_log_info);

    /**
     * @brief Retrieves the current debug logging flag.
     * @return True if debug logging is enabled, false otherwise.
     */
    bool get_log_debug() const;

    /**
     * @brief Sets the debug logging flag.
     * @param a_new_log_debug The new value for the debug logging flag.
     */
    void set_log_debug(bool a_new_log_debug);

    /**
     * @brief Retrieves the current djpg_image_quality.
     * @return jpg_image_quality.
     */
    int get_jpg_image_quality() const;

    /**
     * @brief Sets the jpg_image_quality.
     * @param a_jpg_image_quality The new value for the jpg_image_quality.
     */
    void set_jpg_image_quality(int a_jpg_image_quality);

private:
    bool m_log_info;   ///< Flag to enable or disable info logging.
    bool m_log_debug;  ///< Flag to enable or disable debug logging.
    int m_jpg_image_quality; ///< the image_quality

};

#endif // CONFIG_CONTAINER_H
