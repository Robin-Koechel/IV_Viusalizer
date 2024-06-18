#include "AnnotationWithoutVideo.h"
#include "../Pipeline/RegionOfInterestPipe.h"
#include "../Pipeline/AnnotationPipe.h"
#include "../Pipeline/BlackImagePipe.h"

// Constructor
AnnotationWithoutVideo::AnnotationWithoutVideo() {}

// returns the correct viewType (here ANNOTATION_WITHOUT_VIDEO)
ViewTypeEnum AnnotationWithoutVideo::get_enum()const{
    return ViewTypeEnum::ANNOTATION_WITHOUT_VIDEO;
}

// // creates a pipeline and returns the first pipe object
PipelineObject& AnnotationWithoutVideo::create_pipeline()const{

    RegionOfInterestPipe *a_mask_pipe = new RegionOfInterestPipe(nullptr);
    AnnotationPipe *a_annotation_pipe = new AnnotationPipe(a_mask_pipe);
    BlackImagePipe* a_black_image = new BlackImagePipe(a_annotation_pipe);
    return *a_black_image;
}


AnnotationWithoutVideo::~AnnotationWithoutVideo() {}
