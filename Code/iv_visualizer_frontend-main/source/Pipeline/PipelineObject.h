#ifndef PIPELINEOBJECT_H
#define PIPELINEOBJECT_H

#include "../FrameData/Data/PipelineData.h"

/**
 * @brief Abstract base class representing a pipeline object.
 */
class PipelineObject {
protected:
    PipelineObject* m_next_pipe; /**< Pointer to the next pipeline object. */

public:
    /**
     * @brief Construct a new PipelineObject object.
     * 
     * @param m_next_pipe Pointer to the next pipeline object.
     */
    PipelineObject(PipelineObject* m_next_pipe = nullptr);

    /**
     * @brief Destroy the PipelineObject object.
     */
    ~PipelineObject();

    /**
     * @brief Run the pipeline object.
     * 
     * @param a_data Pipeline data.
     */
    virtual void run(PipelineData& a_data) = 0;
};

#endif // PIPELINEOBJECT_H
