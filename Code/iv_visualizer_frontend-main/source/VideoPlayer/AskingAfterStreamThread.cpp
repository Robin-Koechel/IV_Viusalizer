#include "AskingAfterStreamThread.h"

AskingAfterStreamThread::AskingAfterStreamThread(Stream* a_stream, uint64_t a_time_to_go_in_the_future, uint64_t a_time_to_go_into_the_past)
    : m_stream_to_ask(a_stream), m_time_to_go_in_the_future(a_time_to_go_in_the_future), m_time_to_go_into_the_past(a_time_to_go_into_the_past)
{
}

void AskingAfterStreamThread::slot_ask_for_new_data_frame_stream(uint64_t a_position, const ViewType* a_type)
{
    DataFrameStream* current_data_frame_stream;
    m_current_time_range_of_accessible_data = m_stream_to_ask->get_time_range_of_accessible_data(a_type->get_enum());
    current_data_frame_stream = get_new_stream(a_position, *a_type);
    emit sig_change_data_frame_stream(current_data_frame_stream);
}

void AskingAfterStreamThread::slot_ask_for_new_live_data_frame_stream(uint64_t a_current_position, const ViewType* a_type)
{
    DataFrameStream* current_data_frame_stream = m_stream_to_ask->get_live_stream(a_type->get_enum());
    emit sig_change_data_frame_stream(current_data_frame_stream);
}

DataFrameStream* AskingAfterStreamThread::get_new_stream(uint64_t a_position, const ViewType& a_type)
{
    uint64_t start_time;
    if (0 > static_cast<int64_t>(a_position) - static_cast<int64_t>(m_time_to_go_into_the_past))
    {
        start_time = m_current_time_range_of_accessible_data.get_start_time();
    }
    else
    {
        start_time = m_current_time_range_of_accessible_data.get_start_time() + a_position - m_time_to_go_into_the_past;
    }
    uint64_t end_time;
    if (m_current_time_range_of_accessible_data.get_end_time() < a_position + m_current_time_range_of_accessible_data.get_start_time() + m_time_to_go_in_the_future)
    {
        end_time = m_current_time_range_of_accessible_data.get_end_time();
    }
    else
    {
        end_time = a_position + m_current_time_range_of_accessible_data.get_start_time() + m_time_to_go_in_the_future;
    }
    TimeRange* time_range_to_ask = new TimeRange(start_time, end_time);
    return m_stream_to_ask->get_image_stream(*time_range_to_ask, a_type.get_enum());
}

void AskingAfterStreamThread::slot_change_stream(Stream* a_new_stream)
{
    m_stream_to_ask = a_new_stream;
}
