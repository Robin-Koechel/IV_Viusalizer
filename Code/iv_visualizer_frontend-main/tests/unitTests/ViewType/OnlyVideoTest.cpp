#include "gtest/gtest.h"
#include "AnnotationWithoutVideo.h"
#include "./../../../source/ViewType/OnlyVideo.h"
#include "./../../../source/Pipeline/VideoPipe.h"

TEST(OnlyVideoTest, GetEnum) {
    OnlyVideo onlyVideo;
    EXPECT_EQ(onlyVideo.get_enum(), ViewTypeEnum::ONLY_VIDEO);
}

// Verifies the create_pipeline() method
TEST(OnlyVideoTest, CreatePipeline) {
    OnlyVideo onlyVideo;
    PipelineObject& pipeline = onlyVideo.create_pipeline();
    // Ensure that the pipeline is of type VideoPipe
    EXPECT_TRUE(dynamic_cast<VideoPipe*>(&pipeline) != nullptr);

}