#ifndef PIPELINESCENE_H
#define PIPELINESCENE_H

#include "../FrameData/Data/BoundingBox.h"
#include "qgraphicsscene.h"
#include "../FrameData/Data/RegionOfInterest.h"
#include "../FrameData/Data/Image.h"
#include "RegionOfInterestItem.h"
#include "ColorDict.h"

/**
 * @brief Class representing a pipeline scene.
 */
class PipelineScene : public QGraphicsScene
{
public:
    /**
     * @brief Construct a new PipelineScene object.
     */
    PipelineScene();

    /**
     * @brief Draw a bounding box on the scene.
     * 
     * @param a_bounding_box_to_draw Bounding box to draw.
     */
    void draw_bounding_box(const BoundingBox& a_bounding_box_to_draw);

    /**
     * @brief Draw a region of interest on the scene.
     * 
     * @param a_region_to_draw Region of interest to draw.
     */
    void draw_region_of_interest(const RegionOfInterest& a_region_to_draw);

    /**
     * @brief Draw an image on the scene.
     * 
     * @param a_image_to_draw Image to draw.
     */
    void draw_image(const Image& a_image_to_draw);

    /**
     * @brief Hide bounding boxes that are not valid.
     */
    void hide_not_valid_rects();

    /**
     * @brief Clear all region of interest items from the scene.
     */
    void clear_region_of_interests();

    /**
     * @brief Initialize a new region of interest.
     * 
     * @param a_is_inside Flag indicating if the region is inside or outside.
     * @param a_rect_of_scene the rectangle of the scene
     */
    void init_region_of_interest(bool a_is_inside, QRectF a_rect_of_scene);

    /**
     * @brief Clear the scene.
     */
    void clear();

    /**
     * @brief Clear the scene without clearing the image.
     */
    void clear_without_image();

private:
    QVector<QGraphicsRectItem*> m_current_added_bounding_boxes; /**< Vector containing pointers to current added bounding boxes. */
    RegionOfInterestItem* m_roi_item = nullptr; /**< Pointer to the region of interest item. */
    QGraphicsPixmapItem* m_image = nullptr; /**< Pointer to the image item. */
    int m_valid_rects; /**< Count of valid rectangles. */
    ColorDict* m_color_dict;
};

#endif // PIPELINESCENE_H
