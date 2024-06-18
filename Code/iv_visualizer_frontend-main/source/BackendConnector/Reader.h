#ifndef READER_H
#define READER_H

#include "../FrameData/Config/Config.h"
#include "TimeRange.h"
#include <QString>
#include "../ViewType/ViewTypeEnum.h"
#include "../DataStreams/DataStream.h"

#include <QObject>

/**
 * reader - Abstract class for reading data.
 */
class Reader : public QObject {
    Q_OBJECT
protected:
    QString m_url;

public:
    /**
     * Constructor for Reader.
     *
     * @param a_url The URL.
     */
    Reader(QString a_url);
    
    /**
     * read_images - Reads images.
     *
     * @param a_url_stream The URL of the stream.
     * @param a_time_range The time range.
     * @param a_view_type The view type.
     * @return Pointer to a DataStream.
     */
    virtual DataStream* read_images(const QString a_url_stream, TimeRange& a_time_range, const ViewTypeEnum& a_view_type) = 0;
    
    /**
     * read_config - Reads configuration.
     *
     * @param a_url_stream The URL of the stream.
     * @return Pointer to a Config.
     */
    virtual Config* read_config(const QString a_url_stream) = 0;
    
    /**
     * read_live_images - Reads live images.
     *
     * @param a_url_stream The URL of the stream.
     * @param a_view_type The view type.
     * @return Pointer to a DataStream.
     */
    virtual DataStream* read_live_images(const QString a_url_stream, const ViewTypeEnum& a_view_type) = 0;

    /**
     * @brief reads the range info from the backend
     *
     * @return 2 List of List of timeranges. First List TimeRange of Annotation and second list TimeRange of images.
     */
    virtual QVector<QVector<TimeRange*>>* read_range_info(const QString urlStream) = 0;

    /**
    * @brief Gets the url of the reader.
    * @return The url of the reader.
    */
    QString get_url();
};

#endif // READER_H
