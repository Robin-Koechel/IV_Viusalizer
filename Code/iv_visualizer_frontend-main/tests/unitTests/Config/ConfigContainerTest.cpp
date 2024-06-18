#include <gtest/gtest.h>
#include "./../../../source/FrameData/Config/ConfigContainer.h"

// Test fixture for ConfigContainer class
class ConfigContainerTest : public ::testing::Test {
protected:
    // Set up the test fixture
    void SetUp() override {
        // Initialize ConfigContainer with default settings
        configContainer = ConfigContainer();
    }

    // Tear down the test fixture
    void TearDown() override {
        // Clean up
    }

    // Member variables accessible to all tests in this fixture
    ConfigContainer configContainer;
};

// Test default constructor
TEST_F(ConfigContainerTest, DefaultConstructor) {
    EXPECT_FALSE(configContainer.get_log_info()); // Info logging should be disabled by default
    EXPECT_FALSE(configContainer.get_log_debug()); // Debug logging should be disabled by default
    EXPECT_EQ(configContainer.get_jpg_image_quality(), 0); // Default JPG image quality should be 0
}

// Test parameterized constructor
TEST_F(ConfigContainerTest, ParameterizedConstructor) {
    ConfigContainer configContainerWithParams(true, true);
    EXPECT_TRUE(configContainerWithParams.get_log_info()); // Info logging should be enabled
    EXPECT_TRUE(configContainerWithParams.get_log_debug()); // Debug logging should be enabled
}

// Test set and get log_info
TEST_F(ConfigContainerTest, SetGetLogInfo) {
    configContainer.set_log_info(true);
    EXPECT_TRUE(configContainer.get_log_info()); // Info logging should be enabled
}

// Test set and get log_debug
TEST_F(ConfigContainerTest, SetGetLogDebug) {
    configContainer.set_log_debug(true);
    EXPECT_TRUE(configContainer.get_log_debug()); // Debug logging should be enabled
}

// Test set and get jpg_image_quality
TEST_F(ConfigContainerTest, SetGetJpgImageQuality) {
    configContainer.set_jpg_image_quality(90);
    EXPECT_EQ(configContainer.get_jpg_image_quality(), 90); // JPG image quality should be 90
}
