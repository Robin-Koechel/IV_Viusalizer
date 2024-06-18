#include "RegionOfInterestPainterScene.h"
#include "qgraphicsitem.h"
#include <QGraphicsSceneMouseEvent>

RegionOfInterestPainterScene::RegionOfInterestPainterScene(bool a_interest_is_inside, const QImage& a_background_image, const QVector<RegionOfInterest*>& a_current_polygons) 
: m_current_regions(a_current_polygons), m_is_adding_region(false), m_new_region(nullptr) {
    QGraphicsPixmapItem* m_image = new QGraphicsPixmapItem(QPixmap::fromImage(a_background_image));
    this->addItem(m_image);

    this->update();
    this->init_region_of_interest(a_interest_is_inside, QRectF(0, 0, a_background_image.width(), a_background_image.height()));
    foreach(RegionOfInterest* roi, m_current_regions){
        draw_region_of_interest(*roi);
    }
}
const QPolygon& RegionOfInterestPainterScene::get_new_polygon(){
    m_is_adding_region = false;
    QPolygonF polygonFromItem = this->m_new_region->polygon();

    QPolygon* polygonConverted = new QPolygon();
    foreach (const QPointF& point, polygonFromItem)
    {
        *polygonConverted << point.toPoint();
    }
    return *polygonConverted;
}

void RegionOfInterestPainterScene::new_polygon() {
    m_is_adding_region = true;
    if(m_new_region != nullptr){
        this->removeItem(m_new_region);
        delete m_new_region;
    }
    m_new_region = new QGraphicsPolygonItem();
    m_new_region->setPen(QPen(Qt::green));
    this->addItem(m_new_region);
}


void RegionOfInterestPainterScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (m_is_adding_region) {
        if (event->button() == Qt::LeftButton) {
            QPointF pos = event->scenePos();
            if (!pos.isNull()) {
                QPolygonF polygon = m_new_region->polygon();
                polygon << pos;
                m_new_region->setPolygon(polygon);
            }
        }
    }
}

void RegionOfInterestPainterScene::add_region(RegionOfInterest* a_roi) {
    m_current_regions.append(a_roi);
    draw_region_of_interest(*a_roi);
}


