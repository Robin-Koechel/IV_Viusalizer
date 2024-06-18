#ifndef ANNOTATIONWITHOUTVIDEO_H
#define ANNOTATIONWITHOUTVIDEO_H

#include "ViewType.h"
#include "../FrameData/Data/PipelineData.h"

/**
 * @class AnnotationWithoutVideo
 * @brief Represents a view type that displays annotations without video.
 *
 * This class inherits from ViewType and provides functionality specific
 * to a view type that displays annotations without accompanying video.
 */
class AnnotationWithoutVideo : public ViewType
{
public:
    /**
     * @brief Default constructor.
     */
    AnnotationWithoutVideo();

    /**
     * @brief Destructor.
     */
    ~AnnotationWithoutVideo();

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

#endif // ANNOTATIONWITHOUTVIDEO_H
