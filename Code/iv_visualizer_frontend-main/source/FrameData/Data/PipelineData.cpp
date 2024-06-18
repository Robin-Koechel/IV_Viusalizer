#include "PipelineData.h"
#include <QGraphicsView>
#include <QPixmap>
#include <QImage>
#include <QPainter>

PipelineData::PipelineData(const DataFrame& a_data_frame) : m_data_frame(a_data_frame) {
    m_scene.setBackgroundBrush(default_brush_color);
}

PipelineData::PipelineData() {}

PipelineData::~PipelineData() {}

PipelineScene& PipelineData::get_scene() {
    return m_scene;
}

const DataFrame& PipelineData::get_data_frame() const {
    return m_data_frame;
}

void PipelineData::set_data_frame(const DataFrame& a_new_data_frame) {
    m_data_frame = a_new_data_frame;
}

void PipelineData::clear_regions_of_interest() {
    m_scene.clear_region_of_interests();
}

void PipelineData::clear_scene() {
    m_scene.clear();
}

QImage PipelineData::convert_to_image() {
    const QSize fixedSize(m_data_frame.get_image().width(), m_data_frame.get_image().height());

    QImage image(fixedSize, QImage::Format_RGB888);
    image.fill(Qt::transparent);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    m_scene.render(&painter, QRectF(), QRectF(0, 0, fixedSize.width(), fixedSize.height()));

    painter.end();

    return image;
}

void PipelineData::hide_rects() {
    m_scene.hide_not_valid_rects();
}

void PipelineData::draw_bounding_box(const BoundingBox& a_bounding_box_to_draw) {
    m_scene.draw_bounding_box(a_bounding_box_to_draw);
}

void PipelineData::draw_image(const Image& a_image_to_draw) {
    m_scene.draw_image(a_image_to_draw);
}

void PipelineData::draw_region_of_interest(const RegionOfInterest& a_region_of_interest_to_draw) {
    m_scene.draw_region_of_interest(a_region_of_interest_to_draw);
}

void PipelineData::clear_scene_without_image() {
    m_scene.clear_without_image();
}
