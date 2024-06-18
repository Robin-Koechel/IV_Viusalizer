#ifndef IMAGETOVIDEOCONVERTER_H
#define IMAGETOVIDEOCONVERTER_H

#include <QString>
#include <QImage>
#include "../DataStreams/DataFrameStream.h"
#include "../Pipeline/PipelineObject.h"
#include <iostream>
#include "../DataStreams/DataFrameStream.h"
#include "../FrameData/Data/PipelineData.h"
#include "Encoder.h"
#include <QMutex>
#include "../BackendConnector/Stream.h"
#include "../ViewType/ViewTypeEnum.h"

class ImageToVideoConverter: public QObject{
Q_OBJECT
signals:
    void sig_finished();
    void sig_progress(double progress);
public slots:
    void slot_dataFrameStreamToVideo(DataFrameStream *dataFrameStreamFirst, QString outputFilename, PipelineObject *pipelineoObject, QVector<TimeRange*> ranges, Stream* currentStream, uint64_t askingTimeInNS, TimeRange* rangeToExport, ViewTypeEnum typeToExport);
private:
    TimeRange* findTimeRange(QVector<TimeRange*> ranges,uint64_t position);
    void stop();
public:
    ImageToVideoConverter();
    void request_stop();
private:
    const int update = 250;
    bool m_running;
    QMutex m_mutex;   
};

#endif

