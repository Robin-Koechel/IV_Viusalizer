#ifndef PIPELINE_DATA_H
#define PIPELINE_DATA_H

#include <QGraphicsScene>
#include "DataFrame.h"
#include "../../Pipeline/PipelineScene.h"
#include <QColor>
/**
 * @class PipelineData
 * @brief Manages the visualization data for a processing pipeline.
 *
 * Encapsulates the data necessary for visualizing the output of a processing pipeline,
 * including the scene for drawing and the current data frame. Provides methods to manipulate
 * and visualize the data, such as drawing bounding boxes, images, and regions of interest.
 */
class PipelineData
{
private:
    PipelineScene m_scene;    ///< Scene where visual elements are drawn.
    DataFrame m_data_frame;   ///< Current data frame being visualized.

    const QColor default_brush_color = QColor(0,0,0);
public:
    /**
     * @brief Constructs a PipelineData object with a specified data frame.
     * @param a_data_frame The data frame to visualize.
     */
    explicit PipelineData(const DataFrame& a_data_frame);

    /**
     * @brief Default constructor.
     */
    PipelineData();

    /**
     * @brief Destructor.
     */
    ~PipelineData();

    /**
     * @brief Converts the current visualization to an image.
     * @return QImage representation of the current scene.
     */
    QImage convert_to_image();

    /**
     * @brief Gets the current scene for visualization.
     * @return Reference to the PipelineScene object.
     */
    PipelineScene& get_scene();

    /**
     * @brief Sets the scene for visualization to a new scene.
     * @param a_new_scene The new QGraphicsScene to be used for visualization.
     */
    void set_scene(const QGraphicsScene& a_new_scene);

    /**
     * @brief Draws a bounding box on the scene.
     * @param a_bounding_box_to_draw The BoundingBox object to be drawn.
     */
    void draw_bounding_box(const BoundingBox& a_bounding_box_to_draw);

    /**
     * @brief Draws an image on the scene.
     * @param a_image_to_draw The Image object to be drawn.
     */
    void draw_image(const Image& a_image_to_draw);

    /**
     * @brief Draws a region of interest on the scene.
     * @param a_region_of_interest_to_draw The RegionOfInterest object to be drawn.
     */
    void draw_region_of_interest(const RegionOfInterest& a_region_of_interest_to_draw);

    /**
     * @brief Clears all regions of interest from the scene.
     */
    void clear_regions_of_interest();

    /**
     * @brief Clears the entire scene, removing all visual elements.
     */
    void clear_scene();

    /**
     * @brief Hides all rectangle elements in the scene without removing them.
     */
    void hide_rects();

    /**
     * @brief Clears the scene but retains the background image.
     */
    void clear_scene_without_image();

    /**
     * @brief Gets the current data frame being visualized.
     * @return Constant reference to the current DataFrame object.
     */
    const DataFrame& get_data_frame() const;

    /**
     * @brief Sets the current data frame to a new data frame.
     * @param a_new_data_frame The new DataFrame object to visualize.
     */
    void set_data_frame(const DataFrame& a_new_data_frame);
};

#endif // PIPELINE_DATA_H
