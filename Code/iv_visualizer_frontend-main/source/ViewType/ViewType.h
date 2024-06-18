#ifndef VIEWTYPE_H
#define VIEWTYPE_H

#include "ViewTypeEnum.h"
#include "../Pipeline/PipelineObject.h"

/**
 * @class ViewType
 * @brief Base class for defining different types of views.
 *
 * This class serves as the base class for defining various types of views.
 * It provides an interface for retrieving the view type enumeration and creating
 * pipeline objects specific to each view type.
 */
class ViewType
{
public:
    /**
     * @brief Default constructor.
     */
    ViewType();

    /**
     * @brief Destructor.
     */
    ~ViewType();

    /**
     * @brief Retrieves the enum value corresponding to this view type.
     * @return The enum value representing this view type.
     */
    virtual ViewTypeEnum get_enum() const = 0;

    /**
     * @brief Creates a pipeline object for processing data related to this view type.
     * @return A reference to the created pipeline object.
     */
    virtual PipelineObject& create_pipeline() const = 0;
};

#endif // VIEWTYPE_H
