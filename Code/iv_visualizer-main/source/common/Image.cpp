// Image.cpp
#include "Image.h"

// Constructor with initial data
Image::Image(const uchar* a_data, int a_width, int a_height, uint64_t a_timestamp)
    : QImage(a_data, a_width, a_height, QImage::Format_RGB888), m_timestamp(a_timestamp) {}

// Constructor for an empty image with specified dimensions and a timestamp
Image::Image(int a_width, int a_height, uint64_t a_timestamp) 
    : QImage(a_width, a_height, QImage::Format_RGB888), m_timestamp(a_timestamp) { // TODO remove Format_ARGB32?
    this->fill(Qt::blue); // Fill with blue by default
}

// Default constructor initializes an empty image
Image::Image() : QImage(), m_timestamp(0) {}

Image::Image(QImage* a_img, uint64_t a_timestamp):QImage(*a_img), m_timestamp(a_timestamp){
}

// Destructor
Image::~Image() {
    // Since QImage has its own resource management mechanisms, the destructor of Image does not need to explicitly release resources managed by QImage.
}

// Returns the timestamp attribute
uint64_t Image::get_timestamp() const {
    return m_timestamp;
}

// Getter for the QImage
QImage Image::getQImage() const {
    return *this;
}

// Setter for the timestamp
void Image::setTimeStamp(const uint64_t& timestamp) {
    m_timestamp = timestamp;
}


