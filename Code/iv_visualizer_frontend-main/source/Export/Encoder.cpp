#include "Encoder.h"
#include <iostream>
#include <cstdlib>
#include <QDebug>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h> // Fügen Sie diese Zeile hinzu
}
#include <opencv2/opencv.hpp>
#include <QFile>
// TODO: NEED TO BE TESTED

Encoder::Encoder(QString a_path, int a_fps):m_file_name_path(a_path), m_fps(a_fps){
}

void Encoder::add_image(QImage* a_image){
    if(m_first_image){
        m_file_name_path += ".mp4";
        m_video = new cv::VideoWriter(m_file_name_path.toStdString(), cv::VideoWriter::fourcc('a', 'v', 'c', '1'), m_fps, cv::Size(a_image->width(), a_image->height()));
        m_first_image =false;
    }
    if (!m_video->isOpened()) {
        qDebug() << "Error: Could not open the output video file for writing";
        return;
    }

    cv::Mat cvFrame(a_image->height(), a_image->width(), CV_8UC3, a_image->bits(), a_image->bytesPerLine());

    cv::Mat cvFrameBGR;
    cv::cvtColor(cvFrame, cvFrameBGR, cv::COLOR_RGB2BGR);

    m_video->write(cvFrameBGR);


}

void Encoder::finish_writing_mp4(){
    m_video->release();
}

void Encoder::cancel_writing_mp4() {
    if (m_video && m_video->isOpened()) {
        m_video->release();
        QFile::remove(m_file_name_path);        
    }
}
