#include "Image.h"

// Constructor
Image::Image(const uchar* a_data, int a_width, int a_height, uint64_t a_timestamp)
    : QImage(a_data, a_width, a_height, QImage::Format_RGB888), m_timestamp(a_timestamp) {}

Image::Image(int a_width, int a_height, uint64_t a_timestamp) : QImage(a_width, a_height, QImage::Format_RGB888), m_timestamp(a_timestamp) {
        this->fill(default_background_color);
}

Image::Image(QImage a_img, uint64_t a_timestamp) : QImage(a_img), m_timestamp(a_timestamp){}


Image::Image() : QImage(), m_timestamp(0) {}

Image::~Image() {}

QImage Image::get_qimage() const {
    return *this;
}

uint64_t Image::get_timestamp() const {
    return this->m_timestamp;
}

void Image::set_timestamp(const uint64_t& a_timestamp) {
    m_timestamp = a_timestamp;
}

Image& Image::operator=(const Image& other) {
    if (this != &other) { // Schutz vor Selbstzuweisung
        QImage::operator=(other); // Zuweisung der Basisklasse
        m_timestamp = other.m_timestamp;
        // Kopieren weiterer Daten, falls notwendig
    }
    return *this;
}



