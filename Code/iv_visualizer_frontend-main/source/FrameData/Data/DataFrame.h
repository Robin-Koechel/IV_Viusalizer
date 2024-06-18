#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <QString>
#include "Image.h"
#include <QVector>
#include "RegionOfInterest.h"
#include "BoundingBox.h"

/**
 * @class DataFrame
 * @brief Represents a frame of data, encapsulating an image and its associated metadata.
 *
 * This class is designed to store data related to a single frame, including the image itself,
 * regions of interest (ROIs), and bounding boxes for detected objects. It provides methods to
 * access and modify these properties, supporting the processing and analysis of image data.
 */
class DataFrame
{
private:
    QString m_url; ///< URL from where the image was sourced.
    Image m_image; ///< Image object containing pixel data and metadata.
    QVector<RegionOfInterest*> m_regions_of_interest; ///< List of regions of interest within the image.
    QVector<BoundingBox*> m_list_of_bounding_boxes; ///< List of bounding boxes detected in the image.

public:
    /**
     * @brief Default constructor initializes an empty DataFrame object.
     */
    DataFrame();

    /**
     * @brief Constructs a DataFrame with specified parameters.
     * @param a_url URL from where the image was sourced.
     * @param a_image Image object containing pixel data and metadata.
     * @param a_regions_of_interest List of regions of interest within the image.
     * @param a_bounding_boxes List of bounding boxes detected in the image.
     */
    DataFrame(const QString& a_url, const Image& a_image, 
              const QVector<RegionOfInterest*>& a_regions_of_interest, 
              const QVector<BoundingBox*>& a_bounding_boxes);

    /**
     * @brief Destructor cleans up dynamically allocated resources.
     */
    ~DataFrame();

    /**
     * @brief Gets the image URL.
     * @return Constant reference to the image URL string.
     */
    const QString& get_url() const;

    /**
     * @brief Gets the image object.
     * @return Constant reference to the Image object.
     */
    const Image& get_image() const;

    /**
     * @brief Sets the image object.
     * @param a_new_image New Image object to set.
     */
    void set_image(const Image& a_new_image);

    /**
     * @brief Gets the list of regions of interest.
     * @return Constant reference to the vector of RegionOfInterest pointers.
     */
    const QVector<RegionOfInterest*>& get_regions_of_interest() const;

    /**
     * @brief Sets the list of regions of interest.
     * @param a_new_regions_of_interest New vector of RegionOfInterest pointers to set.
     */
    void set_regions_of_interest(const QVector<RegionOfInterest*>& a_new_regions_of_interest);

    /**
     * @brief Gets the list of bounding boxes.
     * @return Constant reference to the vector of BoundingBox pointers.
     */
    const QVector<BoundingBox*>& get_bounding_boxes() const;

    void setUrl(const QString& a_new_url);

    /**
     * @brief Sets the list of bounding boxes.
     * @param a_new_list_of_bounding_boxes New vector of BoundingBox pointers to set.
     */
    void set_bounding_boxes(const QVector<BoundingBox*>& a_new_list_of_bounding_boxes);
};

#endif // DATA_FRAME_H
