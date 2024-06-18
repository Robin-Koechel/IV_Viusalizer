#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}
#include <opencv2/opencv.hpp>
#include "../FrameData/Data/Image.h"
class Encoder{

    private:
        QString m_file_name_path;
        int m_fps;
        bool m_first_image = true;
        cv::VideoWriter* m_video;

        bool initFrame();


    public:
        Encoder(QString a_path, int a_fps);
        QVector<Image*> get_frames(uint64_t start, uint64_t end);
        void add_image(QImage* a_image);
        void finish_writing_mp4();
        void writeVideo(const QVector<QImage*> &frames, const QString &outputFileName, double fps);
        void cancel_writing_mp4();

};
