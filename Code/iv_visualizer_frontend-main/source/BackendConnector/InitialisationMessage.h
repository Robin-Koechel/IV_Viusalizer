#ifndef INITIALISATION_MESSAGE_H
#define INITIALISATION_MESSAGE_H

#include "../FrameData/Config/Config.h"
#include "TimeRange.h"

/**
 * initialisation_message - Class representing an initialization message.
 */
class InitialisationMessage {
private:
    Config m_config;
    TimeRange m_time_range;

public:
    /**
     * Constructor for InitialisationMessage.
     *
     * @param a_config The configuration.
     * @param a_time_range The time range.
     */
    InitialisationMessage(Config& a_config, TimeRange& a_time_range);
    
    /**
     * get_config - Gets the configuration.
     *
     * @return Reference to the configuration.
     */
    Config& get_config();
    
    /**
     * get_time_range - Gets the time range.
     *
     * @return Reference to the time range.
     */
    TimeRange& get_time_range();
    
    /**
     * Destructor for InitialisationMessage.
     */
    ~InitialisationMessage();
};

#endif // INITIALISATION_MESSAGE_H
