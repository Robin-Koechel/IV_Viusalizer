#include "ConfigGStreamer.h"
ConfigGStreamer::ConfigGStreamer(const std::string& url, int fps, int width, int height)
        : stream_url(url), target_fps(fps), custom_size_width(width), custom_size_height(height) {}


// Getter and setter for stream_url
std::string ConfigGStreamer::getStreamUrl() const {
    return stream_url;
}

void ConfigGStreamer::setStreamUrl(const std::string& value) {
    stream_url = value;
}

// Getter and setter for target_fps
int ConfigGStreamer::getTargetFps() const {
    return target_fps;
}

void ConfigGStreamer::setTargetFps(int value) {
    target_fps = value;
}

// Getter and setter for custom_size_width
int ConfigGStreamer::getCustomSizeWidth() const {
    return custom_size_width;
}

void ConfigGStreamer::setCustomSizeWidth(int value) {
    custom_size_width = value;
}

// Getter and setter for custom_size_height
int ConfigGStreamer::getCustomSizeHeight() const {
    return custom_size_height;
}

void ConfigGStreamer::setCustomSizeHeight(int value) {
    custom_size_height = value;
}