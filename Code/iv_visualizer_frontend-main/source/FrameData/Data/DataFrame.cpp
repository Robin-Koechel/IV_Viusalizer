#include "DataFrame.h"

// Constructor with initial values
DataFrame::DataFrame(const QString& a_url, const Image& a_image, 
                     const QVector<RegionOfInterest*>& a_regions_of_interest, 
                     const QVector<BoundingBox*>& a_bounding_boxes)
    : m_url(a_url), m_image(a_image), m_regions_of_interest(a_regions_of_interest), 
      m_list_of_bounding_boxes(a_bounding_boxes) {
}

// Default constructor
DataFrame::DataFrame() : m_image(Image()) {}

// Destructor
DataFrame::~DataFrame() {
    qDeleteAll(m_list_of_bounding_boxes);
    m_list_of_bounding_boxes.clear();
    qDeleteAll(m_regions_of_interest);
    m_regions_of_interest.clear();
}

// Returns the URL
const QString& DataFrame::get_url() const {
    return m_url;
}

// Sets the URL
void DataFrame::setUrl(const QString& a_new_url) {
    m_url = a_new_url;
}

// returns the image attribute
const Image& DataFrame::get_image() const
{
    return m_image;
}

// Sets the image
void DataFrame::set_image(const Image& a_new_image) {
    m_image = a_new_image;
}

// Returns the set of regions of interest
const QVector<RegionOfInterest*>& DataFrame::get_regions_of_interest() const {
    return m_regions_of_interest;
}

// Sets the set of regions of interest
void DataFrame::set_regions_of_interest(const QVector<RegionOfInterest*>& a_new_regions_of_interest) {
    qDeleteAll(m_regions_of_interest);
    m_regions_of_interest.clear();
    for (RegionOfInterest* roi : a_new_regions_of_interest) {
        m_regions_of_interest.append(new RegionOfInterest(*roi));
    }
}

// Returns the set of bounding boxes
const QVector<BoundingBox*>& DataFrame::get_bounding_boxes() const {
    return m_list_of_bounding_boxes;
}

// Sets the set of bounding boxes
void DataFrame::set_bounding_boxes(const QVector<BoundingBox*>& a_new_list_of_bounding_boxes) {
    qDeleteAll(m_list_of_bounding_boxes);
    m_list_of_bounding_boxes.clear();
    for (BoundingBox* bbox : a_new_list_of_bounding_boxes) {
        m_list_of_bounding_boxes.append(new BoundingBox(*bbox));
    }
}