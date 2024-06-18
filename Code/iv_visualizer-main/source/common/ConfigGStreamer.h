#ifndef CONFIGGSTREAMER_H
#define CONFIGGSTREAMER_H

#include <string>

class ConfigGStreamer {
private:
    std::string stream_url; // e.g., "rtsp://user:pw@ip:port/media"
    int target_fps;
    int custom_size_width;  // -1 means standard resolution
    int custom_size_height; // -1 means standard resolution

public:
    ConfigGStreamer(const std::string& url, int fps, int width, int height);    
    std::string getStreamUrl() const;
    void setStreamUrl(const std::string& value);
    int getTargetFps() const;
    void setTargetFps(int value);
    int getCustomSizeWidth() const;
    void setCustomSizeWidth(int value);
    int getCustomSizeHeight() const;
    void setCustomSizeHeight(int value);
};

#endif // CONFIGGSTREAMER_H
