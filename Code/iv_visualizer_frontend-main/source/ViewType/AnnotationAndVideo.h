#ifndef ANNOTATIONANDVIDEO_H
#define ANNOTATIONANDVIDEO_H

#include "ViewType.h"

/**
 * @class AnnotationAndVideo
 * @brief Represents a view type that combines annotations with video.
 *
 * This class inherits from ViewType and provides functionality specific
 * to a view type that displays annotations overlaid on top of a video.
 */
class AnnotationAndVideo : public ViewType
{
public:
    /**
     * @brief Default constructor.
     */
    AnnotationAndVideo();

    /**
     * @brief Destructor.
     */
    ~AnnotationAndVideo();

    /**
     * @brief Retrieves the enum value corresponding to this view type.
     * @return The enum value representing this view type.
     */
    ViewTypeEnum get_enum() const override;

    /**
     * @brief Creates a pipeline object for processing data related to this view type.
     * @return A reference to the created pipeline object.
     */
    PipelineObject& create_pipeline() const override;
};

#endif // ANNOTATIONANDVIDEO_H
