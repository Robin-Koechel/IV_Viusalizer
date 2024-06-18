#include <gtest/gtest.h>
#include "InitialisationMessage.h"

class InitialisationMessageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any necessary resources before each test
    }

    void TearDown() override {
        // Clean up any allocated resources after each test
    }
};

TEST_F(InitialisationMessageTest, ConstructorAndGetters) {
    // Create a sample Config object
    Config config;

    // Create a sample TimeRange object
    TimeRange timeRange;

    // Create an InitialisationMessage object
    InitialisationMessage initMessage(config, timeRange);

    // Test getters
    EXPECT_EQ(config.get_config_detector().get_cuda_device(), initMessage.get_config().get_config_detector().get_cuda_device());
    EXPECT_EQ(timeRange.get_start_time(), initMessage.get_time_range().get_start_time());
}