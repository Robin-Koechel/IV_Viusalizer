#ifndef CONFIG_GSTREAMER_H
#define CONFIG_GSTREAMER_H

#include <QString>

/**
 * @class ConfigGStreamer
 * @brief Holds configuration settings for GStreamer.
 *
 * This class encapsulates the configuration settings for GStreamer, including
 * stream URL, target FPS, and custom dimensions for the video stream.
 */
class ConfigGStreamer
{
private:
    QString m_stream_url;            ///< The URL of the stream.
    int m_target_fps;                ///< Target frames per second for the stream.
    int m_custom_size_width;         ///< Custom width for the video stream.
    int m_custom_size_height;        ///< Custom height for the video stream.

    /**
     * @brief Default stream URL.
     * 
     * The default URL for the stream.
     */
    const QString m_default_stream_url = "";

    /**
     * @brief Default target frames per second (FPS).
     * 
     * The default target frames per second (FPS) for the stream.
     */
    const int m_default_target_fps = 0;

    /**
     * @brief Default width of the stream.
     * 
     * The default width of the stream.
     */
    const int m_default_size_width = 0;

    /**
     * @brief Default height of the stream.
     * 
     * The default height of the stream.
     */
    const int m_default_size_height = 0;


public:
    /**
     * @brief Constructs a ConfigGStreamer object with default settings.
     */
    ConfigGStreamer();

    /**
     * @brief Constructs a ConfigGStreamer object with the specified settings.
     *
     * This constructor initializes a ConfigGStreamer object with the given stream URL,
     * target frames per second (FPS), and custom size dimensions.
     *
     * @param a_stream_url The URL of the stream.
     * @param a_target_fps The target frames per second (FPS) for the stream.
     * @param a_custom_size_width The custom width dimension for the stream.
     * @param a_custom_size_height The custom height dimension for the stream.
     */
    ConfigGStreamer(QString a_stream_url, int a_target_fps, int a_custom_size_width, int a_custom_size_height);

    ConfigGStreamer& operator=(const ConfigGStreamer& other) {
        if (this != &other) {
            // Copy members from 'other' to 'this'
            m_stream_url = other.m_stream_url;
            m_target_fps = other.m_target_fps;
            m_custom_size_width = other.m_custom_size_width;
            m_custom_size_height = other.m_custom_size_height;
        }
        return *this;
    }

    /**
     * @brief Gets the stream URL.
     * @return The stream URL as a QString.
     */
    QString get_stream_url() const;

    /**
     * @brief Sets the stream URL.
     * @param a_new_stream_url The new stream URL.
     */
    void set_stream_url(const QString& a_new_stream_url);

    /**
     * @brief Gets the target FPS.
     * @return The target FPS as an integer.
     */
    int get_target_fps() const;

    /**
     * @brief Sets the target FPS.
     * @param a_new_target_fps The new target FPS.
     */
    void set_target_fps(int a_new_target_fps);

    /**
     * @brief Gets the custom size width of the video stream.
     * @return The custom size width as an integer.
     */
    int get_custom_size_width() const;

    /**
     * @brief Sets the custom size width of the video stream.
     * @param a_new_custom_size_width The new custom size width.
     */
    void set_custom_size_width(int a_new_custom_size_width);

    /**
     * @brief Gets the custom size height of the video stream.
     * @return The custom size height as an integer.
     */
    int get_custom_size_height() const;

    /**
     * @brief Sets the custom size height of the video stream.
     * @param a_new_custom_size_height The new custom size height.
     */
    void set_custom_size_height(int a_new_custom_size_height);
};

#endif // CONFIG_GSTREAMER_H
