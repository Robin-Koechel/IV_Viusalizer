#include "AnnotationAndVideo.h"
#include "../Pipeline/RegionOfInterestPipe.h"
#include "../Pipeline/AnnotationPipe.h"
#include "../Pipeline/VideoPipe.h"

// Constructor
AnnotationAndVideo::AnnotationAndVideo():ViewType() {}

// returns the correct viewType (here ANNOTATION_AND_VIDEO)
ViewTypeEnum AnnotationAndVideo::get_enum()const{
    return ViewTypeEnum::ANNOTATION_AND_VIDEO;
}

// creates a pipeline and returns the first pipe object
PipelineObject& AnnotationAndVideo::create_pipeline()const{

    // TODO: change pointers
    RegionOfInterestPipe *a_mask_pipe = new RegionOfInterestPipe(nullptr);
    AnnotationPipe *a_annotation_pipe = new AnnotationPipe(a_mask_pipe);
    VideoPipe *a_video_pipe = new VideoPipe(a_annotation_pipe);
    return *a_video_pipe;
}

AnnotationAndVideo::~AnnotationAndVideo() {}

