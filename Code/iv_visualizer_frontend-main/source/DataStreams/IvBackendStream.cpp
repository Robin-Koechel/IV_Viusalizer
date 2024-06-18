#include "IvBackendStream.h"



IvBackendStream::IvBackendStream(IvBackendClient* a_backend_client) : m_running(true), m_backend_client(a_backend_client) {}

IvBackendStream::~IvBackendStream() {
        stop_stream();
        delete m_backend_client; 
}

DataFrame* IvBackendStream::pop(bool a_create_data_frame) {
    QMutexLocker locker(&m_mutex);
    while (m_data.isEmpty() && m_running) {
        m_condNotEmpty.wait(&m_mutex);
    }
    if (!m_running && m_data.isEmpty()) return nullptr; 
    ivbackend::DataStream data = m_data.takeFirst();
    DataFrame* frame = nullptr;
    if(a_create_data_frame){
        frame = create_data_frame(data);
    }
    if (m_data.size() <= size - 1) { 
        m_condNotFull.wakeAll(); 
    }
    return frame;
}

bool IvBackendStream::is_done() {
    QMutexLocker locker(&m_mutex);
    return m_data.isEmpty() && !m_running;
}

void IvBackendStream::stop_stream() {
    QMutexLocker locker(&m_mutex);
    m_running = false;
    m_condNotEmpty.wakeAll(); 
    m_condNotFull.wakeAll();
}

void IvBackendStream::add_data_frame(const ivbackend::DataStream& data) {
    QMutexLocker locker(&m_mutex);
    while (m_data.size() >= size) {
        m_condNotFull.wait(&m_mutex); 
    }
    m_data.append(data);
    if(!m_running){
        m_backend_client->StopLiveStream();
        delete m_backend_client;
    }
    m_condNotEmpty.wakeOne(); 
}

DataFrame* IvBackendStream::create_data_frame(const ivbackend::DataStream& data) {
    MessageConverter converter;
    return new DataFrame(converter.convertImageAndAnnotationToDataFrame(data.data()));
}
