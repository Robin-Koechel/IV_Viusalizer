#ifndef TIMERANGE_H
#define TIMERANGE_H

#include <cstdint>

/**
 * @brief Class representing a time range.
 */
class TimeRange
{
private:
    uint64_t m_timestamp_start; /**< Start timestamp of the time range. */
    uint64_t m_timestamp_end;   /**< End timestamp of the time range. */
    int m_fps; ///<current fps in timerange

public:
    /**
     * @brief Construct a new Time Range object.
     * 
     * @param a_timestamp_start Start timestamp of the time range.
     * @param a_timestamp_end End timestamp of the time range.
     */
    TimeRange(const uint64_t a_timestamp_start, const uint64_t a_timestamp_end);

    /**
     * @brief Construct a new Time Range object with default values.
     */
    TimeRange();

    /**
     * @brief Get the start time of the time range.
     * 
     * @return uint64_t Start timestamp of the time range.
     */
    uint64_t get_start_time();

    /**
     * @brief Get the end time of the time range.
     * 
     * @return uint64_t End timestamp of the time range.
     */
    uint64_t get_end_time();

    /**
     * @brief Set the end time of the time range.
     * 
     * @param a_end_time End timestamp to set.
     */
    void set_time_end(uint64_t a_end_time);

    /**
     * @brief Set the start time of the time range.
     * 
     * @param a_start_time Start timestamp to set.
     */
    void set_start_time(uint64_t a_start_time);

    /**
     * @brief Set the fps of the time range.
     * 
     * @param a_fps fps to set.
     */
    void set_fps(int a_fps);

    /**
     * @brief Get the fps of the time range.
     * 
     * @return int fps of the time range.
     */
    int get_fps();
};

#endif // TIMERANGE_H
