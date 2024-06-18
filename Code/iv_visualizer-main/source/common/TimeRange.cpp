#include "TimeRange.h"

TimeRange::TimeRange(const uint64_t& start, const uint64_t& end, int vid_fps)
    : startTime(start), endTime(end), fps(vid_fps) {}

uint64_t TimeRange::getStartTime() const {
    return startTime;
}

uint64_t TimeRange::getEndTime() const {
    return endTime;
}

int TimeRange::getFps() const {
    return fps;
}

void TimeRange::setFps(int new_fps) {
    fps = new_fps;
}