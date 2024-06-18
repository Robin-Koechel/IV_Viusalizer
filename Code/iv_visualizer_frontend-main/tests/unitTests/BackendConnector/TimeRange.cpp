#include <gtest/gtest.h>
#include "TimeRange.h"

// Test fixture for TimeRange class
class TimeRangeTest : public ::testing::Test {
protected:
    // Set up before each test
    void SetUp() override {
        // Create a TimeRange object with sample timestamps
        start_timestamp = 1000;
        end_timestamp = 2000;
        time_range = new TimeRange(start_timestamp, end_timestamp);
    }

    // Clean up after each test
    void TearDown() override {
        delete time_range;
    }

    // Declare member variables
    TimeRange* time_range;
    uint64_t start_timestamp;
    uint64_t end_timestamp;
};

// Test case to verify the getter for start time
TEST_F(TimeRangeTest, GetStartTime) {
    // Check if the start time returned by the getter matches the sample start timestamp
    EXPECT_EQ(start_timestamp, time_range->get_start_time());
}

// Test case to verify the getter for end time
TEST_F(TimeRangeTest, GetEndTime) {
    // Check if the end time returned by the getter matches the sample end timestamp
    EXPECT_EQ(end_timestamp, time_range->get_end_time());
}

// Test case to verify the setter for start time
TEST_F(TimeRangeTest, SetStartTime) {
    // Set a new start time and verify if the getter returns the updated value
    uint64_t new_start_time = 500;
    time_range->set_start_time(new_start_time);
    EXPECT_EQ(new_start_time, time_range->get_start_time());
}

// Test case to verify the setter for end time
TEST_F(TimeRangeTest, SetEndTime) {
    // Set a new end time and verify if the getter returns the updated value
    uint64_t new_end_time = 3000;
    time_range->set_time_end(new_end_time);
    EXPECT_EQ(new_end_time, time_range->get_end_time());
}

// Test case to verify the setter and getter for FPS
TEST_F(TimeRangeTest, SetAndGetFPS) {
    // Set a sample FPS value and verify if the getter returns the same value
    int sample_fps = 30;
    time_range->set_fps(sample_fps);
    EXPECT_EQ(sample_fps, time_range->get_fps());
}