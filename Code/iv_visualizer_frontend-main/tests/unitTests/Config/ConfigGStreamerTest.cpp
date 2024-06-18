#include <gtest/gtest.h>
#include "./../../../source/FrameData/Config/ConfigGStreamer.h"

// Test fixture for ConfigGStreamer class
class ConfigGStreamerTest : public ::testing::Test {
protected:
    // Set up the test fixture
    void SetUp() override {
        // Initialize ConfigGStreamer with default settings
        configGStreamer = ConfigGStreamer();
    }

    // Tear down the test fixture
    void TearDown() override {
        // Clean up
    }

    // Member variables accessible to all tests in this fixture
    ConfigGStreamer configGStreamer;
};

// Test default constructor
TEST_F(ConfigGStreamerTest, DefaultConstructor) {
    EXPECT_EQ(configGStreamer.get_stream_url(), ""); // Default stream URL should be empty
    EXPECT_EQ(configGStreamer.get_target_fps(), 0); // Default target FPS should be 0
    EXPECT_EQ(configGStreamer.get_custom_size_width(), 0); // Default custom size width should be 0
    EXPECT_EQ(configGStreamer.get_custom_size_height(), 0); // Default custom size height should be 0
}

// Test parameterized constructor
TEST_F(ConfigGStreamerTest, ParameterizedConstructor) {
    ConfigGStreamer configGStreamerWithParams("http://example.com/stream", 30, 640, 480);
    EXPECT_EQ(configGStreamerWithParams.get_stream_url(), "http://example.com/stream"); // Stream URL should match
    EXPECT_EQ(configGStreamerWithParams.get_target_fps(), 30); // Target FPS should be 30
    EXPECT_EQ(configGStreamerWithParams.get_custom_size_width(), 640); // Custom size width should be 640
    EXPECT_EQ(configGStreamerWithParams.get_custom_size_height(), 480); // Custom size height should be 480
}

// Test set and get stream_url
TEST_F(ConfigGStreamerTest, SetGetStreamUrl) {
    configGStreamer.set_stream_url("http://example.com/stream");
    EXPECT_EQ(configGStreamer.get_stream_url(), "http://example.com/stream"); // Stream URL should match
}

// Test set and get target_fps
TEST_F(ConfigGStreamerTest, SetGetTargetFps) {
    configGStreamer.set_target_fps(30);
    EXPECT_EQ(configGStreamer.get_target_fps(), 30); // Target FPS should be 30
}

// Test set and get custom_size_width
TEST_F(ConfigGStreamerTest, SetGetCustomSizeWidth) {
    configGStreamer.set_custom_size_width(640);
    EXPECT_EQ(configGStreamer.get_custom_size_width(), 640); // Custom size width should be 640
}

// Test set and get custom_size_height
TEST_F(ConfigGStreamerTest, SetGetCustomSizeHeight) {
    configGStreamer.set_custom_size_height(480);
    EXPECT_EQ(configGStreamer.get_custom_size_height(), 480); // Custom size height should be 480
}