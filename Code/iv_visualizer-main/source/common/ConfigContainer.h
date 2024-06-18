#ifndef CONFIGCONTAINER_H
#define CONFIGCONTAINER_H

class ConfigContainer {
private:
    bool log_info;
    bool log_debug;
    int jpg_image_quality; // Range: 0-100 or -1

public:
    // Default constructor
    ConfigContainer();

    // Constructor with attribute initialization
    ConfigContainer(bool logInfo, bool logDebug, int jpgQuality);
    
    // Getter and setter methods
    bool getLogInfo() const;
    void setLogInfo(bool value);
    bool getLogDebug() const;
    void setLogDebug(bool value);
    int getJpgImageQuality() const;
    void setJpgImageQuality(int value);
};

#endif // CONFIGCONTAINER_H
