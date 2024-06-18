#include "gtest/gtest.h"
#include "./../../../source/ViewType/AnnotationWithoutVideo.h"
#include "./../../../source/Pipeline/AnnotationPipe.h"
#include "./../../../source/Pipeline/RegionOfInterestPipe.h"

TEST(AnnotationWithoutVideoTest, GetEnum) {
    AnnotationWithoutVideo annotationWithoutVideo;
    EXPECT_EQ(annotationWithoutVideo.get_enum(), ViewTypeEnum::ANNOTATION_WITHOUT_VIDEO);
}

