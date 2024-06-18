#ifndef WRITER_H
#define WRITER_H

#include "../FrameData/Config/Config.h"
#include <QString>
#include "InitialisationMessage.h"

/**
 * @brief Abstract class representing a writer.
 */
class Writer
{
protected:
    QString m_url; /**< URL of the writer. */

public:
    /**
     * @brief Construct a new Writer object.
     * 
     * @param a_url URL of the writer.
     */
    Writer(QString a_url);

    /**
     * @brief Virtual function to write configuration.
     * 
     * @param a_url_stream URL of the stream.
     * @param a_config_to_write Configuration to write.
     */
    virtual void write_config(const QString a_url_stream,Config& a_config_to_write) = 0;

    /**
     * @brief Virtual function to add a camera.
     * 
     * @param a_url_stream URL of the stream.
     * @return true if connection was established.
     */
    virtual bool add_cam(const QString a_url_stream) = 0;

    /**
    * @brief Gets the url of the writer.
    * @return The url of the writer.
    */
    QString get_url();
    
    /**
     * @brief stops the stream
     * @param a_url_stream the url to stop
    */
    virtual void stop_cam(const QString a_url_stream) = 0;
};

#endif // WRITER_H
