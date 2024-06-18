#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QColor>

/**
 * @class Image
 * @brief Extends QImage with additional timestamp information.
 *
 * This class is derived from QImage and adds a timestamp attribute to
 * provide temporal context for the image. It is useful for applications
 * where images are part of a time-sequenced dataset.
 */
class Image : public QImage {
private:
    uint64_t m_timestamp;
    const QColor default_background_color = Qt::blue;

public:

    Image& operator=(const Image& other);
    
    /**
     * @brief Constructs an Image object with pixel data, dimensions, and a timestamp.
     * @param a_data Pointer to the raw image data.
     * @param a_width Width of the image in pixels.
     * @param a_height Height of the image in pixels.
     * @param a_timestamp Timestamp associated with the image.
     */
    Image(const uchar* a_data, int a_width, int a_height, uint64_t a_timestamp);

    /**
     * @brief Constructs an Image object with dimensions, and a timestamp, but no initial data.
     * @param a_width Width of the image in pixels.
     * @param a_height Height of the image in pixels.
     * @param a_timestamp Timestamp associated with the image.
     */
    Image(int a_width, int a_height, uint64_t a_timestamp);

    Image(QImage a_img, uint64_t a_timestamp);

    /**
     * @brief Default constructor, initializes an empty image with no timestamp.
     */
    Image();

    /**
     * @brief Destructor.
     */
    ~Image();

    /**
     * @brief Getter for the QImage.
     * 
     * @return The QImage object representing the image.
     */
    QImage get_qimage() const;

    /**
     * @brief Getter for the timestamp.
     * 
     * @return The timestamp of the image.
     */
    uint64_t get_timestamp() const;

    /**
     * @brief Setter for the timestamp.
     * 
     * @param timestamp The timestamp to set.
     */
    void set_timestamp(const uint64_t& a_timestamp);

};

#endif // IMAGE_H
