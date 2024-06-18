#include <gtest/gtest.h>
#include "Log.h"
#include "./../../../source/Log/LogType.h"

// Test fixture for Log
class LogTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up any objects or variables needed for tests
        logType = LogType::DEBUG;
        description = "Test description";
        timestamp = 1234567890;
    }

    void TearDown() override {
    }

    LogType logType;
    QString description;
    uint64_t timestamp;
};

// Test Log constructor
TEST_F(LogTest, ConstructorTest) {
    Log log(logType, description, timestamp);
    // Add assertions to check the behavior of the constructor
    ASSERT_EQ(log.get_log_type(), logType);
    ASSERT_EQ(log.get_description(), description);
    ASSERT_EQ(log.get_timestamp(), timestamp);
}

// Test Log destructor (if any cleanup logic is added in the future)
TEST_F(LogTest, DestructorTest) {
    // No explicit test is needed for destructor if it doesn't have specific logic
    ASSERT_TRUE(true);
}

// Test Log::get_description
TEST_F(LogTest, GetDescriptionTest) {
    Log log(logType, description, timestamp);
    // Add assertions to check the behavior of the get_description method
    ASSERT_EQ(log.get_description(), description);
}

// Test Log::get_log_type
TEST_F(LogTest, GetLogTypeTest) {
    Log log(logType, description, timestamp);
    // Add assertions to check the behavior of the get_log_type method
    ASSERT_EQ(log.get_log_type(), logType);
}

// Test Log::get_timestamp
TEST_F(LogTest, GetTimestampTest) {
    Log log(logType, description, timestamp);
    // Add assertions to check the behavior of the get_timestamp method
    ASSERT_EQ(log.get_timestamp(), timestamp);
}
