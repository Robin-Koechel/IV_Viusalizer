#include "PipelineScene.h"
#include "qgraphicsitem.h"


PipelineScene::PipelineScene()
{
    m_valid_rects = 0;
    this->m_image = nullptr;
    m_color_dict = ColorDict::get_color_dict();
}


void PipelineScene::draw_bounding_box(const BoundingBox& a_bounding_box_to_draw){


   
    std::vector<int> a_rect = a_bounding_box_to_draw.get_coords(BoundingBox::Format::TLWH);
    QPen pen = QPen(m_color_dict->get_color(a_bounding_box_to_draw.get_label()));
    //pen.setWidth(1); 

    if (m_valid_rects >= this->m_current_added_bounding_boxes.size()) {
        QGraphicsRectItem* a_rect_item = new QGraphicsRectItem(a_rect[0], a_rect[1], a_rect[2], a_rect[3]);
        a_rect_item->setPen(pen);
        this->addItem(a_rect_item);
        this->m_current_added_bounding_boxes.append(a_rect_item);
    } else {
        this->m_current_added_bounding_boxes.at(m_valid_rects)->setRect(a_rect[0], a_rect[1], a_rect[2], a_rect[3]);
        this->m_current_added_bounding_boxes.at(m_valid_rects)->setVisible(true);
        this->m_current_added_bounding_boxes.at(m_valid_rects)->setZValue(1);
        m_current_added_bounding_boxes.at(m_valid_rects)->setPen(pen);

    }
     m_valid_rects++;

}

void PipelineScene::init_region_of_interest(bool a_is_inside, QRectF a_rect_of_scene){
    m_roi_item = new RegionOfInterestItem(a_is_inside, a_rect_of_scene);
    this->addItem(m_roi_item);
}
void PipelineScene::draw_region_of_interest(const RegionOfInterest& a_region_of_interest_to_draw){
    m_roi_item->draw_region_of_interest(a_region_of_interest_to_draw);
}
void PipelineScene::draw_image(const Image& a_image_to_draw){
    qreal a_z_of_image = -1;
    if(this->m_image != nullptr){
        this->removeItem(m_image);
        a_z_of_image = m_image->zValue();
        delete m_image;
    }
    m_image = new QGraphicsPixmapItem(QPixmap::fromImage(a_image_to_draw));
    this->addItem(m_image);
    if(a_z_of_image != -1){
        m_image->setZValue(a_z_of_image);
    }
    a_z_of_image = m_image->zValue();
}


void PipelineScene::hide_not_valid_rects(){
    for(int i = m_valid_rects; i < this->m_current_added_bounding_boxes.size(); i++){
        this->m_current_added_bounding_boxes.at(m_valid_rects)->setVisible(false);
    }
    m_valid_rects = 0;
}
void PipelineScene::clear_region_of_interests(){
    if (m_roi_item && this->items().contains(m_roi_item)) {
        this->removeItem(m_roi_item);
        delete m_roi_item;
    }
}

void PipelineScene::clear_without_image(){
    clear_region_of_interests();
    foreach(QGraphicsItem* a_item, this->m_current_added_bounding_boxes) {
        this->removeItem(a_item);
        delete a_item;
    }
    this->m_current_added_bounding_boxes.clear();
}

void PipelineScene::clear(){
    foreach(QGraphicsItem* a_item, this->m_current_added_bounding_boxes) {
        this->removeItem(a_item);
        delete a_item;
    }
    this->m_current_added_bounding_boxes.clear();
    clear_region_of_interests();
    if(this->m_image != nullptr && this->items().contains(m_image)){
        this->removeItem(m_image);
        delete m_image;
        m_image = nullptr;
    }
}
