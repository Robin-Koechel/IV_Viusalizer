#include "RegionOfInterest.h"

RegionOfInterest::~RegionOfInterest() {}

bool RegionOfInterest::is_inside() const { return m_is_inside; }

const QPolygon& RegionOfInterest::get_polygon() const { return m_polygon; }

const QString& RegionOfInterest::get_name() const { return m_name; }

int RegionOfInterest::get_id() const { return m_id; }

const QColor& RegionOfInterest::get_color() const { return m_color; }

float RegionOfInterest::get_opacity() const { return m_opacity; }

void RegionOfInterest::set_name(const QString& a_name) { m_name = a_name; }

void RegionOfInterest::set_id(const int a_id) { m_id = a_id; }

void RegionOfInterest::set_polygon(const QPolygon& a_polygon) {
    m_polygon = a_polygon;
}

void RegionOfInterest::set_is_inside(const bool a_is_inside) {
    m_is_inside = a_is_inside;
}

void RegionOfInterest::set_color(const QColor& a_color) { m_color = a_color; }

void RegionOfInterest::set_opacity(const float a_opacity) {
    m_opacity = a_opacity;
}

RegionOfInterest::RegionOfInterest(const QString& a_name, const int a_id,
                                   const QPolygon& a_polygon,
                                   const bool a_is_inside,
                                   const float a_opacity) {
    m_is_inside = a_is_inside;
    m_polygon = a_polygon;
    m_name = a_name;
    m_id = a_id;
    m_color = default_color;
    m_opacity = a_opacity;
}


RegionOfInterest::RegionOfInterest(){
    m_is_inside = false;
    m_polygon = QPolygon();
    m_name = "";
    m_id = 0;
    m_color = QColor(Qt::black);
    m_opacity = 1.0;
}
