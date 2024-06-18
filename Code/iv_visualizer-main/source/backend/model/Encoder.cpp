#include "Encoder.h"
#include <iostream>
#include <cstdlib>
#include <QDebug>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
}
#include <opencv2/opencv.hpp>
#include <fstream>


Encoder::Encoder(uint64_t a_start_timestamp, QString a_path, int a_fps) : m_video_start_timestamp(a_start_timestamp), m_file_name_path(a_path), m_fps(a_fps){
    m_video = nullptr;
    avformat_network_init();
    m_frame_counter = 0;
}

Encoder::~Encoder() {
  if (m_video != nullptr) {
    if (m_video->isOpened()) {
      finish_writing_mp4();
      delete m_video;
    }
  }
}

QVector<std::shared_ptr<Image>> Encoder::get_frames(uint64_t a_start_frame, uint64_t a_end_frame) {
    uint64_t a_start = a_start_frame;
    uint64_t a_end = a_end_frame;

    QString a_video_file_path = m_file_name_path;
    QVector<std::shared_ptr<Image>> a_timestamp_image_vec;


    AVFormatContext* a_p_format_ctx = avformat_alloc_context();
    if (avformat_open_input(&a_p_format_ctx, a_video_file_path.toStdString().c_str(), nullptr, nullptr) != 0) {
        return a_timestamp_image_vec; // Couldn't open video file
    }

    if (avformat_find_stream_info(a_p_format_ctx, nullptr) < 0) {
        avformat_close_input(&a_p_format_ctx);
        return a_timestamp_image_vec; // Couldn't find m_stream information
    }

    int a_video_stream_index = -1;
    for (unsigned int i = 0; i < a_p_format_ctx->nb_streams; i++) {
        if (a_p_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            a_video_stream_index = i;
            break;
        }
    }

    if (a_video_stream_index == -1) {
        avformat_close_input(&a_p_format_ctx);
        return a_timestamp_image_vec; // Didn't find a video m_stream
    }
    AVStream* video_stream = a_p_format_ctx->streams[a_video_stream_index];
    AVRational frame_rate = video_stream->avg_frame_rate;
    double fps = static_cast<double>(frame_rate.num) / frame_rate.den;

    m_fps = fps;
    AVCodecParameters* a_codec_parameters = a_p_format_ctx->streams[a_video_stream_index]->codecpar;
    AVCodec* a_codec = avcodec_find_decoder(a_codec_parameters->codec_id);
    if (!a_codec) {
        avformat_close_input(&a_p_format_ctx);
        return a_timestamp_image_vec; // Codec not found
    }

    AVCodecContext* m_codec_context = avcodec_alloc_context3(a_codec);
    if (avcodec_parameters_to_context(m_codec_context, a_codec_parameters) < 0) {
        avformat_close_input(&a_p_format_ctx);
        avcodec_free_context(&m_codec_context);
        return a_timestamp_image_vec; // Failed to copy a_codec context
    }

    if (avcodec_open2(m_codec_context, a_codec, nullptr) < 0) {
        avformat_close_input(&a_p_format_ctx);
        avcodec_free_context(&m_codec_context);
        return a_timestamp_image_vec; // Could not open a_codec
    }

    AVFrame* m_frame = av_frame_alloc();
    AVFrame* a_frame_rgb = av_frame_alloc();
    if (!m_frame || !a_frame_rgb) {
        avformat_close_input(&a_p_format_ctx);
        avcodec_free_context(&m_codec_context);
        av_frame_free(&m_frame);
        av_frame_free(&a_frame_rgb);
        return a_timestamp_image_vec; // Failed to allocate m_frame
    }

    int a_num_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, m_codec_context->width, m_codec_context->height, 32);
    uint8_t* a_buffer = (uint8_t*)av_malloc(a_num_bytes * sizeof(uint8_t));
    av_image_fill_arrays(a_frame_rgb->data, a_frame_rgb->linesize, a_buffer, AV_PIX_FMT_RGB24, m_codec_context->width, m_codec_context->height, 32);

    struct SwsContext* a_sws_ctx = sws_getContext(
        m_codec_context->width,
        m_codec_context->height,
        m_codec_context->pix_fmt,
        m_codec_context->width,
        m_codec_context->height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        nullptr,
        nullptr,
        nullptr
    );

    AVPacket a_packet;
    m_frame_counter = 0;
    while (av_read_frame(a_p_format_ctx, &a_packet) >= 0) {
        if (a_packet.stream_index == a_video_stream_index) {
            if (avcodec_send_packet(m_codec_context, &a_packet) == 0) {
                while (avcodec_receive_frame(m_codec_context, m_frame) == 0) {
                    if (m_frame_counter + m_video_start_timestamp >= a_start && m_frame_counter + m_video_start_timestamp <= a_end) {
                        sws_scale(a_sws_ctx, (const uint8_t* const*)m_frame->data, m_frame->linesize, 0, m_codec_context->height, a_frame_rgb->data, a_frame_rgb->linesize);
                        QImage a_q_img(a_frame_rgb->data[0], m_codec_context->width, m_codec_context->height, a_frame_rgb->linesize[0], QImage::Format_RGB888);
                        std::shared_ptr<Image> a_img = std::make_shared<Image>(new QImage(a_q_img.copy()), m_video_start_timestamp + m_frame_counter);
                        a_timestamp_image_vec.push_back(a_img);
                    }
                    m_frame_counter += 1000000000/m_fps;
                    if (m_frame_counter + m_video_start_timestamp > a_end){
                        break;
                    }
                }
            }
        }
        av_packet_unref(&a_packet);
        if (m_frame_counter > a_end) break;
    }

    av_free(a_buffer);
    av_frame_free(&m_frame);
    av_frame_free(&a_frame_rgb);
    avcodec_close(m_codec_context);
    avformat_close_input(&a_p_format_ctx);
    avcodec_free_context(&m_codec_context);
    sws_freeContext(a_sws_ctx);

    return a_timestamp_image_vec;
}

void Encoder::add_image(std::shared_ptr<Image> a_image){
    if(m_first_image){
        m_file_name_path = m_file_name_path + m_video_name;
        m_video = new cv::VideoWriter(m_file_name_path.toStdString(), cv::VideoWriter::fourcc('a', 'v', 'c', '1'), m_fps, cv::Size(a_image->width(), a_image->height()));
        m_first_image =false;
        m_video_start_timestamp = a_image->get_timestamp();
        this->m_width = a_image->width();
        this->m_height = a_image->height();
    }

    if (!m_video->isOpened()) {
        qDebug() << "Error: Could not open the output video file for writing";
        return;
    }

    cv::Mat cvFrame(a_image->height(), a_image->width(), CV_8UC3, a_image->bits(), a_image->bytesPerLine());
    cv::Mat cvFrameRGB;
    cv::cvtColor(cvFrame, cvFrameRGB, cv::COLOR_RGB2BGR); 
    m_video->write(cvFrameRGB);
}

void Encoder::finish_writing_mp4(){
    m_video->release();
}

int Encoder::get_fps(){
    if(m_fps == 0){
        std::string videoPathStdString = m_file_name_path.toStdString();
        cv::VideoCapture cap(videoPathStdString);
        if (!cap.isOpened()) {
            std::cerr << "Error: Unable to open video." << std::endl;
            m_fps= 0;
        }
        m_fps = cap.get(cv::CAP_PROP_FPS);
        cap.release();
    }
    return this->m_fps;
}

int Encoder::get_width(){
    return this->m_width;
}

int Encoder::get_height(){
    return this->m_height;
}

QString Encoder::get_path(){
    return this->m_file_name_path;
}

uint64_t Encoder::get_timestamp() {
    return m_video_start_timestamp;
}

uint64_t Encoder::get_frame_count() {
    if(m_frame_counter == 0){
       std::string videoPathStdString = m_file_name_path.toStdString();
        cv::VideoCapture cap(videoPathStdString);
        if (!cap.isOpened()) {
            std::cerr << "Error: Unable to open video." << std::endl;
            m_frame_counter = 0;
        }
        uint64_t frameCount = cap.get(cv::CAP_PROP_FRAME_COUNT);
        cap.release();
        m_frame_counter = frameCount * (1000000000/get_fps());
    }
    return m_frame_counter;
}