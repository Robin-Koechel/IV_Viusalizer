#include "ImageToVideoConverter.h"
#include <iostream>
#include "../DataStreams/DataFrameStream.h"
#include "../FrameData/Data/PipelineData.h"
#include "../BackendConnector/TimeRange.h"

ImageToVideoConverter::ImageToVideoConverter() {

}


TimeRange* ImageToVideoConverter::findTimeRange(QVector<TimeRange*> ranges,uint64_t position){
    for(int i = 0; i < ranges.size(); i++){
        if(ranges.at(i)->get_start_time() <= position && ranges.at(i)->get_end_time() >= position){
            return ranges.at(i);
        }
    }
    std::cout << "Could not find Timerange" << std::endl;
    return nullptr;
}
// TODO: add framerate
void ImageToVideoConverter::slot_dataFrameStreamToVideo( DataFrameStream *dataFrameStreamFirst, QString outputFilename, PipelineObject *pipelineoObject, QVector<TimeRange*> ranges, Stream* currentStream, uint64_t askingTimeInNS, TimeRange* rangeToExport, ViewTypeEnum typeToExport){
    DataFrame *dataFrame = dataFrameStreamFirst->get_next_data_frame(true);
    TimeRange* time = findTimeRange(ranges, dataFrame->get_image().get_timestamp());
    int currentfps = time->get_fps();
    Encoder encoder(outputFilename, currentfps);
    DataFrameStream* dataFrameStream = dataFrameStreamFirst;
    int number_of_files = 1;
    int current_width = dataFrame->get_image().width();
    int current_height = dataFrame->get_image().height();
    TimeRange* range = rangeToExport;
    uint64_t difference_of_time = range->get_end_time() - range->get_start_time();
    uint64_t start = range->get_start_time();
    PipelineData pipeData(*dataFrame);

    int frame = 0;
    m_running = true;
    uint64_t currentTimestamp = start;
    do{
        do{
            pipelineoObject->run(pipeData);
            QImage tempImage = pipeData.convert_to_image();
            if(time->get_start_time() <= pipeData.get_data_frame().get_image().get_timestamp() && pipeData.get_data_frame().get_image().get_timestamp() <= time->get_end_time()
                && current_height == pipeData.get_data_frame().get_image().height() && current_width == pipeData.get_data_frame().get_image().width()){
                encoder.add_image(&tempImage);
            }
            else{
                std::cout << "Add new File" << std::endl;
                encoder.finish_writing_mp4();
                outputFilename = outputFilename.left(outputFilename.length() - 4) + QString::number(number_of_files) + ".mp4";
                TimeRange* time = findTimeRange(ranges, dataFrame->get_image().get_timestamp());
                int currentfps = time->get_fps();
                Encoder encoder(outputFilename, currentfps);
                number_of_files++;
            }
            pipeData.clear_regions_of_interest();
            pipeData.hide_rects();
            DataFrame* next_data_frame = dataFrameStream->get_next_data_frame(true);
            currentTimestamp = next_data_frame->get_image().get_timestamp();
            pipeData.set_data_frame(*next_data_frame);
            if(update == frame){
                frame = 0;
                double result = static_cast<double>(pipeData.get_data_frame().get_image().get_timestamp() - start) / static_cast<double>(difference_of_time);
                emit sig_progress(result);
            }
            frame++;
            QMutexLocker locker(&m_mutex);
            if (!m_running) {
                delete dataFrameStream;
                delete pipelineoObject;
                pipeData.set_data_frame(*new DataFrame());
                encoder.cancel_writing_mp4();
                return;
            }
            locker.unlock();
        }while(dataFrameStream->next_data_frame_is_in_stream());
        delete dataFrameStream;
        TimeRange timeRangeToUse = TimeRange(currentTimestamp, currentTimestamp + askingTimeInNS);
        dataFrameStream = currentStream->get_image_stream(timeRangeToUse, typeToExport);
        DataFrame* next_data_frame = dataFrameStream->get_next_data_frame(true);
        currentTimestamp = next_data_frame->get_image().get_timestamp();
        pipeData.set_data_frame(*next_data_frame);
    }while(currentTimestamp < range->get_end_time());
   
    encoder.finish_writing_mp4();
    delete dataFrameStream;
    delete pipelineoObject;
    emit sig_finished();
    pipeData.set_data_frame(*new DataFrame());
}

void ImageToVideoConverter::request_stop(){
    QMutexLocker locker(&m_mutex);
    m_running = false;
}
