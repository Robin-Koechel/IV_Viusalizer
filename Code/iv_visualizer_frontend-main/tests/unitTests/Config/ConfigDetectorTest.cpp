#include <gtest/gtest.h>
#include "./../../../source/FrameData/Config/ConfigDetector.h"

// Test fixture for ConfigDetector class
class ConfigDetectorTest : public ::testing::Test {
protected:
    // Set up the test fixture
    void SetUp() override {
        // Initialize ConfigDetector with default settings
        configDetector = ConfigDetector();
    }

    // Tear down the test fixture
    void TearDown() override {
        // Clean up
    }

    // Member variables accessible to all tests in this fixture
    ConfigDetector configDetector;
};

// Test default constructor
TEST_F(ConfigDetectorTest, DefaultConstructor) {
    EXPECT_EQ(configDetector.get_cuda_device(), 0); // Default CUDA device should be 0
    EXPECT_DOUBLE_EQ(configDetector.get_nms_threshold(), 0.0); // Default NMS threshold should be 0.0
    EXPECT_EQ(configDetector.get_confidence_threshold(), 0); // Default confidence threshold should be 0
    EXPECT_EQ(configDetector.get_rois().size(), 0); // There should be no regions of interest initially
    EXPECT_EQ(configDetector.get_inference_type(), InferenceType::ONNX_CPU); // Default inference type should be ONNX_CPU
}

// Test parameterized constructor
TEST_F(ConfigDetectorTest, ParameterizedConstructor) {
    QVector<RegionOfInterest*> rois;
    ConfigDetector configDetectorWithParams(1, 0.5, 50, InferenceType::ONNX_GPU , rois);
    EXPECT_EQ(configDetectorWithParams.get_cuda_device(), 1); // CUDA device should be 1
    EXPECT_DOUBLE_EQ(configDetectorWithParams.get_nms_threshold(), 0.5); // NMS threshold should be 0.5
    EXPECT_EQ(configDetectorWithParams.get_confidence_threshold(), 50); // Confidence threshold should be 50
    EXPECT_EQ(configDetectorWithParams.get_inference_type(), InferenceType::ONNX_GPU); // Inference type should be CUDA_GPU
}

// Test set and get cuda_device
TEST_F(ConfigDetectorTest, SetGetCudaDevice) {
    configDetector.set_cuda_device(2);
    EXPECT_EQ(configDetector.get_cuda_device(), 2); // CUDA device should be 2
}

// Test set and get nms_threshold
TEST_F(ConfigDetectorTest, SetGetNmsThreshold) {
    configDetector.set_nms_threshold(0.75);
    EXPECT_DOUBLE_EQ(configDetector.get_nms_threshold(), 0.75); // NMS threshold should be 0.75
}

// Test set and get confidence_threshold
TEST_F(ConfigDetectorTest, SetGetConfidenceThreshold) {
    configDetector.set_confidence_threshold(75);
    EXPECT_EQ(configDetector.get_confidence_threshold(), 75); // Confidence threshold should be 75
}

// Test set and get rois
TEST_F(ConfigDetectorTest, SetGetRois) {
    QVector<RegionOfInterest*> rois;
    rois.push_back(new RegionOfInterest(/* parameters for RegionOfInterest */));
    configDetector.set_rois(rois);
    EXPECT_EQ(configDetector.get_rois().size(), 1); // There should be one region of interest
}

// Test set and get inference_type
TEST_F(ConfigDetectorTest, SetGetInferenceType) {
    configDetector.set_inference_type(InferenceType::TRT_FLOAT32);
    EXPECT_EQ(configDetector.get_inference_type(), InferenceType::TRT_FLOAT32); // Inference type should be TRT_FLOAT32
}
