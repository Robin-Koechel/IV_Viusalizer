#ifndef ENCODER_H
#define ENCODER_H

#include <iostream>
#include <vector>
#include <memory>

extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

#include <opencv2/opencv.hpp>
#include "Image.h"

/**
 * @class Encoder
 * @brief Class for encoding images into video.
 */
class Encoder {

private:
    QString m_file_name_path; /**< File path where the video will be saved. */
    QString m_video_name = ".mp4"; /**< Default video file name. */
    uint64_t m_video_start_timestamp; /**< Start timestamp of the video. */
    int m_fps; /**< Frames per second for the video. */
    AVFormatContext* m_format_context = nullptr; /**< Format context for video encoding. */
    AVCodecContext* m_codec_context = nullptr; /**< Codec context for video encoding. */
    AVStream* m_stream = nullptr; /**< Video stream. */
    SwsContext* m_sws_context = nullptr; /**< Sws context for frame conversion. */
    AVFrame* m_frame = nullptr; /**< Video frame. */
    uint64_t m_frame_counter = 0; /**< Counter for frames. */
    bool m_first_image = true; /**< Flag indicating first image. */
    cv::VideoWriter* m_video; /**< Video writer object. */
    int m_width;
    int m_height;


public:
    /**
     * @brief Constructor for Encoder class.
     * @param a_path Path where the video will be saved.
     * @param a_fps Frames per second for the video.
     */
    Encoder(uint64_t a_start_timestamp, QString a_path, int a_fps);
    ~Encoder();

    /**
     * @brief Get frames within a specified range.
     * @param a_start Start timestamp.
     * @param a_end End timestamp.
     * @return Vector of Image pointers.
     */
    QVector<std::shared_ptr<Image>> get_frames(uint64_t a_start_frame, uint64_t a_end_frame);

    /**
     * @brief Add an image to the video.
     * @param a_image Pointer to the Image object.
     */
    void add_image(std::shared_ptr<Image> a_image);

    /**
     * @brief Finish writing the MP4 video.
     */
    void finish_writing_mp4();

    int get_fps();

    int get_width();

    int get_height();

    QString get_path();

    uint64_t get_timestamp();

    uint64_t get_frame_count();
};

#endif // ENCODER_H
