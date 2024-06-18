#ifndef REGIONOFINTERESTPAINTERSCENE_H
#define REGIONOFINTERESTPAINTERSCENE_H

#include "../Pipeline/PipelineScene.h"
#include "qgraphicsitem.h"
#include "qgraphicsscene.h"
#include "qpolygon.h"
#include "../FrameData/Data/RegionOfInterest.h"

/**
 * @class RegionOfInterestPainterScene
 * @brief The RegionOfInterestPainterScene class is designed for drawing and managing regions of interest (ROIs) on a graphical scene.
 * 
 * This class extends PipelineScene to support interactive addition, visualization, and management of regions of interest
 * within an image or a scene. It allows users to dynamically create and modify polygons representing ROIs.
 */
class RegionOfInterestPainterScene : public PipelineScene
{
public:
    /**
     * @brief Constructor for a RegionOfInterestPainterScene with specified parameters.
     * @param a_interest_is_inside Determines whether the interest region is considered to be inside the polygon.
     * @param a_background_image The background image over which regions are drawn.
     * @param a_current_polygons A vector of existing RegionOfInterest objects to be initially displayed.
     */
    RegionOfInterestPainterScene(bool a_interest_is_inside, const QImage& a_background_image, const QVector<RegionOfInterest *> &a_current_polygons);

    /**
     * @brief Returns the polygon that is currently being drawn.
     * @return The new polygon as a QPolygon reference.
     */
    const QPolygon &get_new_polygon();

    /**
     * @brief Initiates the creation of a new polygon for defining a region of interest.
     */
    void new_polygon();

    /**
     * @brief Handles mouse press events to facilitate polygon creation and editing.
     * @param event The QGraphicsSceneMouseEvent containing details about the mouse press event.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    /**
     * @brief Adds a new RegionOfInterest to the scene.
     * @param a_roi Pointer to the RegionOfInterest object to be added.
     */
    void add_region(RegionOfInterest* a_roi);

private:
    QVector<RegionOfInterest*> m_current_regions; ///< Vector storing the current regions of interest.
    bool m_is_adding_region; ///< Flag indicating if a new region is being added.
    QGraphicsPolygonItem* m_new_region; ///< Pointer to the graphical polygon item representing the new region.

};

#endif // REGIONOFINTERESTPAINTERSCENE_H
