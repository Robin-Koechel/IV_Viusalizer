// TODO: remove from project
#include "gtest/gtest.h"
#include "TimeRange.h" 

class TimeRangeTest : public ::testing::Test {
protected:
};

TEST_F(TimeRangeTest, ConstructorSetsStartAndEndTime) {
    uint64_t start = 1709091819000000;
    uint64_t end = 1709091819000005;

    TimeRange timeRange(start, end, 0);

    EXPECT_EQ(timeRange.getStartTime(), start);
    EXPECT_EQ(timeRange.getEndTime(), end);
}

TEST_F(TimeRangeTest, StartTimeBeforeEndTime) {
    uint64_t start = 1709091819000000;
    uint64_t end = 1709091819000005;

    TimeRange timeRange(start, end, 0);

    EXPECT_TRUE(timeRange.getStartTime() < timeRange.getEndTime());
}
