#include "gtest/gtest.h"
#include "AnnotationAndVideo.h"
#include "./../../../source/Pipeline/VideoPipe.h"
#include "./../../../source/Pipeline/AnnotationPipe.h"
#include "./../../../source/Pipeline/RegionOfInterestPipe.h"

// Verifies the get_enum() method
TEST(AnnotationAndVideoTest, GetEnum) {
    AnnotationAndVideo annotationAndVideo;
    EXPECT_EQ(annotationAndVideo.get_enum(), ViewTypeEnum::ANNOTATION_AND_VIDEO);
}

// Verifies the create_pipeline() method
TEST(AnnotationAndVideoTest, CreatePipeline) {
    AnnotationAndVideo annotationAndVideo;
    PipelineObject& pipeline = annotationAndVideo.create_pipeline();
    // Ensure that the pipeline is of type VideoPipe
    EXPECT_TRUE(dynamic_cast<VideoPipe*>(&pipeline) != nullptr);

}
