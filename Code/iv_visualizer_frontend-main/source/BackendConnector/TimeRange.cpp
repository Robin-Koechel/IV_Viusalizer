#include "TimeRange.h"

TimeRange::TimeRange(const uint64_t a_timestamp_start, const uint64_t a_timestamp_end)
{
    this->m_timestamp_start = a_timestamp_start;
    this->m_timestamp_end = a_timestamp_end;
}

uint64_t TimeRange::get_end_time(){
    return m_timestamp_end;
}

uint64_t TimeRange::get_start_time(){
    return m_timestamp_start;
}

void TimeRange::set_start_time(uint64_t a_start_time){
    m_timestamp_start = a_start_time;
}


void TimeRange::set_time_end(uint64_t endTime){
    m_timestamp_end= endTime;
}

TimeRange::TimeRange()
{

}

 void TimeRange::set_fps(int a_fps){
    m_fps = a_fps;
 }

   
int TimeRange::get_fps(){
    return m_fps;
}
