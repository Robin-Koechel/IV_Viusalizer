#include "OnlyVideo.h"
#include "../Pipeline/RegionOfInterestPipe.h"
#include "../Pipeline/VideoPipe.h"

// Constructor
OnlyVideo::OnlyVideo() {}

OnlyVideo::~OnlyVideo(){}

// returns the correct viewType (here ONLY_VIDEO)
ViewTypeEnum OnlyVideo::get_enum()const{
    return ViewTypeEnum::ONLY_VIDEO;
}

// creates a pipeline and returns the first pipe object
PipelineObject& OnlyVideo::create_pipeline()const{

    RegionOfInterestPipe *a_mask_pipe = new RegionOfInterestPipe(nullptr);
    VideoPipe *a_video_pipe = new VideoPipe(a_mask_pipe);
    return *a_video_pipe;
}
