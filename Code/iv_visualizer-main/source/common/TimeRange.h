
#ifndef TIMERANGE_H
#define TIMERANGE_H

#include <QDateTime>
#include <iterator>

class TimeRange {
private:
    uint64_t startTime;
    uint64_t endTime;
    int fps;

public:
    TimeRange(const uint64_t& start, const uint64_t& end, int fps);
    uint64_t getStartTime() const;
    uint64_t getEndTime() const;
    int getFps() const;
    void setFps(int fps);
};

#endif // TIMERANGE_H
