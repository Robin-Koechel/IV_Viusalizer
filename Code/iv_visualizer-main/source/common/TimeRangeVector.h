#pragma once
#include <vector>
#include "TimeRange.h"
#include <iterator>

class TimeRangeVector {
public:
    using TimeRangeVectorType = std::vector<std::vector<TimeRange>>;

    TimeRangeVector() = default;
    TimeRangeVector(const TimeRangeVectorType& timeRanges);

    TimeRangeVectorType& getTimeRanges();
    void setTimeRanges(const TimeRangeVectorType& timeRanges);
    
    // Add a method to add a single TimeRange to the TimeRangeVector
    void addTimeRange(const TimeRange& timeRange);

private:
    TimeRangeVectorType timeRanges;
};