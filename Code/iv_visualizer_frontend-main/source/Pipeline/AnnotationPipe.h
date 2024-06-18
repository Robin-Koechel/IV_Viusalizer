#ifndef ANNOTATIONPIPE_H
#define ANNOTATIONPIPE_H

#include "PipelineObject.h"
#include <QColor>

class PipelineData;

/**
 * @brief Class representing an annotation pipe in a pipeline.
 */
class AnnotationPipe: public PipelineObject
{
private:

public:
    /**
     * @brief Construct a new AnnotationPipe object.
     * 
     * @param a_next_pipe Pointer to the next pipeline object.
     */
    AnnotationPipe(PipelineObject* a_next_pipe = nullptr);

    /**
     * @brief Destroy the AnnotationPipe object.
     */
    virtual ~AnnotationPipe();

    /**
     * @brief Run the annotation pipe.
     * 
     * @param a_data Pipeline data.
     */
    virtual void run(PipelineData& a_data) override;
};

#endif // ANNOTATIONPIPE_H
