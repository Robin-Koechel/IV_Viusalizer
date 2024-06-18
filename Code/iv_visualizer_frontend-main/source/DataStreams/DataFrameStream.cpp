#include "DataFrameStream.h"
#include <algorithm> 

DataFrameStream::DataFrameStream(DataStream* a_data_stream, uint64_t a_end_time, int a_size_of_ring_buffer)
: m_data_stream(a_data_stream) {
    m_current_data_frames = new DataFrameRingbuffer(a_size_of_ring_buffer);
    m_time_range_of_stream = new TimeRange(get_next_data_frame(true)->get_image().get_timestamp(), a_end_time);
}

DataFrameStream::~DataFrameStream() {
    delete m_current_data_frames;
    delete m_time_range_of_stream;
}

bool DataFrameStream::contains_data_frame(uint64_t a_position) {
    update_time();
    return m_time_range_of_stream->get_start_time() <= a_position && a_position <= m_time_range_of_stream->get_end_time();
}

bool DataFrameStream::is_almost_finished(uint64_t a_time_remaining, uint64_t a_current_time) {
    update_time();
    if(m_time_range_of_stream->get_end_time() <= a_current_time + a_time_remaining){
        return true;
    }
    else{
        return false;
    }

}

DataFrame* DataFrameStream::get_data_frame(uint64_t a_position) {
    while (!m_current_data_frames->contains_data_frame(a_position)) {
        DataFrame* next_data_frame;
        do{
            next_data_frame = m_data_stream->pop(true);
            if(!next_data_frame){
                break;
            }
        }
        while(next_data_frame->get_image().get_timestamp() == 0);
        if (next_data_frame) {
            m_current_data_frames->push(next_data_frame);
        } else {
            break;
        }
    }
    return m_current_data_frames->search_ring_buffer(a_position);
}

DataFrame* DataFrameStream::get_next_data_frame(bool a_create_data_frame) {
    if (!m_current_data_frames->next_data_frame_is_in_buffer()) {
         DataFrame* next_data_frame;
        do{
            next_data_frame = m_data_stream->pop(true);
            if(!next_data_frame){
                return nullptr;
            }
        }
        while(next_data_frame->get_image().get_timestamp() == 0);   
        if (next_data_frame) {          
            m_current_data_frames->push(next_data_frame);        
        }
        else{
            return nullptr;
        }
    }
    if(a_create_data_frame){
        DataFrame* next_data_frame = m_current_data_frames->get_next_data_frame();
        return next_data_frame;
    }
    else{
        return nullptr;
    }

}

TimeRange* DataFrameStream::get_time_of_stream(){
    return m_time_range_of_stream;
}

bool DataFrameStream::next_data_frame_is_in_stream() {
    return !m_data_stream->is_done() || m_current_data_frames->next_data_frame_is_in_buffer();
}

void DataFrameStream::update_time() {
    if (m_current_data_frames->get_size() > 0) {
        m_time_range_of_stream->set_start_time(m_current_data_frames->get_start_time());
    }
}

void DataFrameStream::stop_stream() {
    m_data_stream->stop_stream();
}


DataFrame* DataFrameStream::get_previous_data_frame(){
    return m_current_data_frames->get_previous_data_frame();
}

bool DataFrameStream::is_previous_data_frame_in_stream(){
    return m_current_data_frames->is_previous_data_frame_in_stream();
}
