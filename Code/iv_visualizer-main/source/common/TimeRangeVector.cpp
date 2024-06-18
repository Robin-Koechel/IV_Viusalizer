#include "TimeRangeVector.h"

TimeRangeVector::TimeRangeVector(const TimeRangeVectorType& timeRanges)
    : timeRanges(timeRanges) {}

TimeRangeVector::TimeRangeVectorType& TimeRangeVector::getTimeRanges() {
    return timeRanges;
}

void TimeRangeVector::setTimeRanges(const TimeRangeVectorType& newTimeRanges) {
    timeRanges = newTimeRanges;
}

void TimeRangeVector::addTimeRange(const TimeRange& timeRange) {
    // Add the provided TimeRange to the TimeRangeVector
    timeRanges.push_back({timeRange});
}