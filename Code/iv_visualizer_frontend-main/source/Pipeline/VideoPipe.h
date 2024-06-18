#ifndef VIDEOPPIPE_H
#define VIDEOPPIPE_H

#include "PipelineObject.h"

/**
 * @brief Class representing a video pipe in a pipeline.
 */
class VideoPipe : public PipelineObject {
public:
    /**
     * @brief Construct a new VideoPipe object.
     * 
     * @param m_next_pipe Pointer to the next pipeline object.
     */
    VideoPipe(PipelineObject* m_next_pipe = nullptr);

    /**
     * @brief Destroy the VideoPipe object.
     */
    ~VideoPipe();

    /**
     * @brief Run the video pipe.
     * 
     * @param a_data Pipeline data.
     */
    void run(PipelineData& a_data) override;
};

#endif // VIDEOPPIPE_H
