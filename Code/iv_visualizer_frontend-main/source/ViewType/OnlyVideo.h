#ifndef ONLYVIDEO_H
#define ONLYVIDEO_H

#include "ViewTypeEnum.h"
#include "ViewType.h"

/**
 * @class OnlyVideo
 * @brief Represents a view type that displays only video.
 *
 * This class inherits from ViewType and provides functionality specific
 * to a view type that displays video without any additional annotations.
 */
class OnlyVideo : public ViewType
{
public:
    /**
     * @brief Default constructor.
     */
    OnlyVideo();

    /**
     * @brief Destructor.
     */
    ~OnlyVideo();

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

#endif // ONLYVIDEO_H
