#ifndef BLACKPIPE_H
#define BLACKPIPE_H

#include "PipelineObject.h"
#include <QColor>

class PipelineData;

/**
 * @brief Class representing an annotation pipe in a pipeline.
 */
class BlackImagePipe: public PipelineObject
{
private:
    Image* m_black_image;

public:
    /**
     * @brief Construct a new BlackImagePipe object.
     * 
     * @param a_next_pipe Pointer to the next pipeline object.
     */
    BlackImagePipe(PipelineObject* a_next_pipe = nullptr);

    /**
     * @brief Destroy the BlackImagePipe object.
     */
    virtual ~BlackImagePipe();

    /**
     * @brief Run the black image pipe.
     * 
     * @param a_data Pipeline data.
     */
    virtual void run(PipelineData& a_data) override;
};

#endif // BLACKPIPE_H
