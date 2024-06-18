#include "DataFrameRingBuffer.h"
#include <cstdlib>
#include <iostream>
#include <QtConcurrent/QtConcurrent>

DataFrameRingbuffer::DataFrameRingbuffer(int a_capacity) 
    : m_current_time(0,0), m_capacity(a_capacity), m_last_index(-1), m_start_index(0), m_last_selected_index(-1) {
    connect(&m_future_watcher, &QFutureWatcher<void>::finished, this, &DataFrameRingbuffer::slot_cleanup);
}

DataFrameRingbuffer::~DataFrameRingbuffer() {
    for (DataFrame* a_frame : m_ring_buffer) {
        if(a_frame != nullptr){
            delete a_frame;
        }
        a_frame = nullptr;
    }
    m_ring_buffer.clear();
}

void DataFrameRingbuffer::delete_frames_async() {
    QFuture<void> a_future = QtConcurrent::run([this]() { this->delete_frames(); });
    m_future_watcher.setFuture(a_future);
}

void DataFrameRingbuffer::delete_frames() {
    for (DataFrame* a_frame : m_ring_buffer) {
        delete a_frame;
        a_frame = nullptr;
    }
    m_ring_buffer.clear();
}

void DataFrameRingbuffer::slot_cleanup() {
    delete this;
}

bool DataFrameRingbuffer::is_previous_data_frame_in_stream(){
    if(m_last_selected_index < 0 || (m_last_selected_index == 0 && m_ring_buffer.size() < m_capacity)){
        return false;
    }
    if(m_ring_buffer[minus_one_modulo(m_last_selected_index, m_capacity)]->get_image().get_timestamp() <= m_ring_buffer[m_last_selected_index]->get_image().get_timestamp()){
        return true;
    }
    return false;
}


DataFrame* DataFrameRingbuffer::get_previous_data_frame(){
    m_last_selected_index = minus_one_modulo(m_last_selected_index, m_capacity);
    return m_ring_buffer[m_last_selected_index];
}


int DataFrameRingbuffer::minus_one_modulo(int a_number_to_sub, int a_modulo){
    if(a_number_to_sub == 0){
        return a_modulo - 1;
    }
    return a_number_to_sub - 1;
}

DataFrame* DataFrameRingbuffer::search_ring_buffer(uint64_t a_position) {
    int a_start = 0;
    int a_end = m_ring_buffer.size() - 1;
    uint64_t a_nearest_diff = UINT64_MAX;
    int a_nearest_index = -1;

    while (a_start <= a_end) {
        int a_mid = a_start + (a_end - a_start) / 2;
        int real_mid = (a_mid + m_start_index) % m_capacity;
        uint64_t a_diff = std::abs(static_cast<int64_t>(m_ring_buffer[real_mid]->get_image().get_timestamp()) - static_cast<int64_t>(a_position));

        if (a_diff < a_nearest_diff) {
            a_nearest_diff = a_diff;
            a_nearest_index = real_mid;
        }

        if (m_ring_buffer[real_mid]->get_image().get_timestamp() < a_position) {
            a_start = a_mid + 1;
        } else {
            a_end = a_mid - 1;
        }
    }
    
    m_last_selected_index = a_nearest_index;
    return m_ring_buffer[a_nearest_index];
}

void DataFrameRingbuffer::push(DataFrame* a_to_insert) {
    m_last_index = (m_last_index + 1) % m_capacity;

    if (m_ring_buffer.size() >= m_capacity) {
        if(m_ring_buffer[m_last_index] != nullptr){
            delete m_ring_buffer[m_last_index];
        }
        m_ring_buffer[m_last_index] = nullptr; 
    }
    
    if (m_ring_buffer.size() < m_capacity) {
        m_ring_buffer.push_back(a_to_insert);
    } else {
        m_ring_buffer[m_last_index] = a_to_insert;
    }

    if (m_ring_buffer.size() == 1) {
        m_current_time.set_start_time(a_to_insert->get_image().get_timestamp());
    }
    m_current_time.set_time_end(a_to_insert->get_image().get_timestamp());

    if (m_ring_buffer.size() == m_capacity) {
        m_start_index = (m_start_index + 1) % m_capacity;
    }
}

const DataFrame& DataFrameRingbuffer::get_last_added_data_frame() {
    return *m_ring_buffer[m_last_index];
}

bool DataFrameRingbuffer::contains_data_frame(uint64_t a_position) {
    return a_position <= m_current_time.get_end_time();
}

uint64_t DataFrameRingbuffer::get_start_time() {
    m_current_time.set_start_time(m_ring_buffer[(m_last_index + 1) % m_ring_buffer.size()]->get_image().get_timestamp());
    return m_current_time.get_start_time();
}

DataFrame* DataFrameRingbuffer::get_next_data_frame() {
    m_last_selected_index = (m_last_selected_index + 1) % m_capacity;
    return m_ring_buffer[m_last_selected_index];
}

bool DataFrameRingbuffer::next_data_frame_is_in_buffer() {
    if(m_last_selected_index + 1 >= m_ring_buffer.size() || m_last_selected_index == -1){
        return false;
    }
    if(m_ring_buffer[(m_last_selected_index + 1) % m_capacity]->get_image().get_timestamp() >= m_ring_buffer[m_last_selected_index]->get_image().get_timestamp()){
        return true;
    }
    return false;
}

int DataFrameRingbuffer::get_size() {
    return m_ring_buffer.size();
}

int DataFrameRingbuffer::get_capacity() {
    return m_capacity;
}
