#include "VideoPlayer.h"

VideoPlayer::VideoPlayer(uint64_t a_buffer_time)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &VideoPlayer::slot_on_timer_timeout);
    m_buffer_time = a_buffer_time;
    m_data_frame_stream = nullptr;
    m_blocking_pos = false;
    m_current_speed = 1;
    m_is_playing = true;


}

void VideoPlayer::show_current_data_frame()
{
    if(m_current_data_frame != nullptr){
        emit sig_show_data_frame(m_current_data_frame, m_blocking_pos, m_current_version);
        emit sig_pos_changed(m_current_data_frame->get_image().get_timestamp());
        m_blocking_pos = false;
    }
}

void VideoPlayer::slot_one_frame_forwards(){
    m_current_data_frame = m_next_data_frame;
    m_next_data_frame = m_data_frame_stream->get_next_data_frame(true);
    if(m_data_frame_stream->is_almost_finished(m_buffer_time, m_current_data_frame->get_image().get_timestamp())){
        emit sig_ask_for_new_data_frame_stream();
    }
    show_current_data_frame();
}


void VideoPlayer::slot_one_frame_backwards(){
    if(m_data_frame_stream->is_previous_data_frame_in_stream()){
        m_next_data_frame = m_current_data_frame;
        m_current_data_frame = m_data_frame_stream->get_previous_data_frame();
        show_current_data_frame();
    }
    else{
        emit sig_ask_for_new_data_frame_stream();
    }
}


void VideoPlayer::start_timer()
{
    uint64_t current_timestamp = m_current_data_frame->get_image().get_timestamp();
    uint64_t next_timestamp = m_next_data_frame->get_image().get_timestamp();
    uint64_t difference_of_time_stamps = calculate_time_stamp_difference_in_ms(current_timestamp, next_timestamp);
    int timer = static_cast<int>((difference_of_time_stamps)/(m_current_speed));
    m_timer->start(timer);
    m_current_data_frame = m_next_data_frame;
    int number_to_pop = std::max(1,static_cast<int>(m_current_speed));
    for(int i = 0; i < number_to_pop - 1; i++){
        m_next_data_frame = m_data_frame_stream->get_next_data_frame(false);
        if(m_data_frame_stream->is_almost_finished(m_buffer_time, m_current_data_frame->get_image().get_timestamp())){
            emit sig_ask_for_new_data_frame_stream();
            break;
        }
    }
    m_next_data_frame = m_data_frame_stream->get_next_data_frame(true);
    if(m_next_data_frame == nullptr){
        emit sig_ask_for_new_data_frame_stream();
        m_timer->stop();
    }


}

void VideoPlayer::slot_on_timer_timeout()
{
    if(m_is_playing){
        start_timer();
    }    
    show_current_data_frame();
}

uint64_t VideoPlayer::calculate_time_stamp_difference_in_ms(uint64_t a_start, uint64_t a_end)
{
    return (a_end - a_start) / 1000000; 
}

void VideoPlayer::slot_speed_changed(double a_speed)
{
    m_current_speed = a_speed;
}

void VideoPlayer::slot_play_or_pause(bool a_playing)
{
    m_timer->stop();
    if(!a_playing){
        m_is_playing = false;
    }
    else{
        start_timer();
        m_is_playing = true;

    }
}

void VideoPlayer::slot_change_data_frame_stream(DataFrameStream* a_new_stream, uint64_t a_current_position)
{
    m_timer->stop();
    if(m_data_frame_stream != nullptr){
        m_data_frame_stream->stop_stream();
        delete m_data_frame_stream;
    }
    m_data_frame_stream = a_new_stream;
    m_current_data_frame = m_data_frame_stream->get_data_frame(a_current_position);
    m_next_data_frame = m_data_frame_stream->get_next_data_frame(true);
    m_current_version++;
    show_current_data_frame();
    if(m_next_data_frame == nullptr){
        emit sig_ask_for_new_data_frame_stream();
        return;
    }
    if(m_is_playing){
        start_timer();
    }
}

void VideoPlayer::slot_pos_changed(uint64_t a_new_pos)
{
    m_timer->stop();
    m_blocking_pos = true;
    if(m_data_frame_stream->contains_data_frame(a_new_pos)){
        m_current_data_frame = m_data_frame_stream->get_data_frame(a_new_pos);
        if(m_data_frame_stream->next_data_frame_is_in_stream()){
            m_next_data_frame = m_data_frame_stream->get_next_data_frame(true);
            show_current_data_frame();
            start_timer();
        }
    }
    else{
        emit sig_ask_for_new_data_frame_stream();
    }
}
