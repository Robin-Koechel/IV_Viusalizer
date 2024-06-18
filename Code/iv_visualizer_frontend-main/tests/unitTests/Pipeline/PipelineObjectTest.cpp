#include <gtest/gtest.h>
#include "PipelineObject.h"
#include "./../../../source/Pipeline/AnnotationPipe.h"
#include "./../../../source/Pipeline/RegionOfInterestPipe.h"
#include "./../../../source/Pipeline/VideoPipe.h"
#include "gmock/gmock.h"

class MockBoundingBox : public BoundingBox {
public:
    MOCK_METHOD(void, draw, (), (const));
};

class PipelineObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        annotation_pipe = new AnnotationPipe(nullptr);
        region_of_interest_pipe = new RegionOfInterestPipe(annotation_pipe);
        video_pipe = new VideoPipe(region_of_interest_pipe);
    }

    void TearDown() override {
        delete video_pipe;
    }
    AnnotationPipe* annotation_pipe;
    RegionOfInterestPipe* region_of_interest_pipe;
    VideoPipe* video_pipe;
};


TEST_F(PipelineObjectTest, AnnotationPipeTest){
    // TODO: write Test
}

TEST_F(PipelineObjectTest, RegionOfInterestTest){
    // TODO: write Test
}

TEST_F(PipelineObjectTest, VideoPipeTest){
    // TODO: write Test
}
