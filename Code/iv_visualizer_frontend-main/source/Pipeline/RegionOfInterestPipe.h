#ifndef REGIONOFINTERESTPIPE_H
#define REGIONOFINTERESTPIPE_H

#include "../Pipeline/PipelineObject.h"
#include "../FrameData/Data/PipelineData.h"

/**
 * @brief Class representing a region of interest pipe in a pipeline.
 */
class RegionOfInterestPipe : public PipelineObject {
public:
    /**
     * @brief Construct a new RegionOfInterestPipe object.
     * 
     * @param m_next_pipe Pointer to the next pipeline object.
     */
    RegionOfInterestPipe(PipelineObject* m_next_pipe = nullptr);

    /**
     * @brief Destroy the RegionOfInterestPipe object.
     */
    ~RegionOfInterestPipe();

    /**
     * @brief Run the region of interest pipe.
     * 
     * @param a_data Pipeline data.
     */
    void run(PipelineData& a_data) override;
};

#endif // REGIONOFINTERESTPIPE_H
