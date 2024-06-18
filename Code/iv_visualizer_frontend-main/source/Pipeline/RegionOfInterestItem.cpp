#include "RegionOfInterestItem.h"
#include "qpainter.h"

RegionOfInterestItem::RegionOfInterestItem(bool a_interest_is_inside, const QRectF &a_bounds, QGraphicsItem *a_parent)
    : QGraphicsItem(a_parent), m_region_of_interest_rect(a_bounds), m_interest_is_inside(a_interest_is_inside) {
    if (a_interest_is_inside) {
        m_region_of_interest_path.addRect(a_bounds);
    }
}

QRectF RegionOfInterestItem::boundingRect() const {
    return m_region_of_interest_rect;
}

void RegionOfInterestItem::draw_region_of_interest(const RegionOfInterest &a_roi) {
    QPainterPath a_polygon_path;
    a_polygon_path.addPolygon(a_roi.get_polygon());

    QColor a_color = QColor(a_roi.get_color());
    a_color.setAlpha(255 * a_roi.get_opacity());

    if (m_interest_is_inside) {

        m_region_of_interest_path = m_region_of_interest_path.subtracted(a_polygon_path);
    } else {
        m_region_of_interest_path.addPath(a_polygon_path);
    }

    m_paths_with_colors.push_back(std::make_pair(a_polygon_path, a_color));

    update();
}

void RegionOfInterestItem::paint(QPainter *a_painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if(m_interest_is_inside){
        a_painter->setBrush(QBrush(color_with_highest_alpha()));
        a_painter->setPen(Qt::NoPen);
        a_painter->drawPath(m_region_of_interest_path);
    }
    else{
        for (const auto &a_path_with_color : m_paths_with_colors) {
            a_painter->setBrush(QBrush(a_path_with_color.second));
            a_painter->setPen(Qt::NoPen);
            a_painter->drawPath(a_path_with_color.first);
        }
    }

}


QColor RegionOfInterestItem::color_with_highest_alpha() {
    if (m_paths_with_colors.empty()) {
        return QColor();
    }
    QColor a_max_alpha_color = m_paths_with_colors[0].second;
    int a_max_alpha = a_max_alpha_color.alpha();
    for (const auto& pathWithColor : m_paths_with_colors) {
        const QColor& a_color = pathWithColor.second;
        if (a_color.alpha() > a_max_alpha) {
            a_max_alpha = a_color.alpha();
            a_max_alpha_color = a_color;
        }
    }
    return a_max_alpha_color;
}
