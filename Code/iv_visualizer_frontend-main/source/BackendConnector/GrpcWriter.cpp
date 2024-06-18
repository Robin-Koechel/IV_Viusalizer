#include "GrpcWriter.h"
#include "ivbackend.grpc.pb.h"
#include <QtConcurrent/QtConcurrent>


GRPCWriter::GRPCWriter(QString a_url_grpc) : Writer(a_url_grpc)
{
    m_backend = new IvBackendClient();
    m_backend->SetServerAddress(a_url_grpc.toStdString());
}

void GRPCWriter::write_config(const QString urlStream, Config& configToWrite){
    ivbackend::ConfigContainer container = MessageConverter::createConfigContainer(configToWrite.get_config_container().get_log_info(), configToWrite.get_config_container().get_log_debug(), 
        configToWrite.get_config_container().get_jpg_image_quality());
        
    ivbackend::ConfigDetector detector = MessageConverter::createConfigDetector(MessageConverter::getInferenceOutOfEnum(configToWrite.get_config_detector().get_inference_type()), configToWrite.get_config_detector().get_cuda_device(), 
        configToWrite.get_config_detector().get_nms_threshold(), configToWrite.get_config_detector().get_confidence_threshold());
    
    ivbackend::RoiList roiList;
    roiList.MergeFrom(MessageConverter::convertRegionsToRoiList(configToWrite.get_config_detector().get_rois()));
    ivbackend::RoiList* mutableRoiList = detector.mutable_rois();
    if (mutableRoiList != nullptr) {
        mutableRoiList->CopyFrom(roiList);
    }
    
    ivbackend::ConfigGStreamer g_streamer = MessageConverter::createConfigGStreamer(
        configToWrite.get_config_gstreamer().get_stream_url().toStdString(), 
        configToWrite.get_config_gstreamer().get_target_fps(), configToWrite.get_config_gstreamer().get_custom_size_width(), 
        configToWrite.get_config_gstreamer().get_custom_size_height());
    ivbackend::StreamIdentifier streamIdentifier;
    streamIdentifier.set_url(urlStream.toStdString());
    m_backend->SetConfig(MessageConverter::createConfigResponseMessage(container, detector, g_streamer), streamIdentifier);
}

bool GRPCWriter::add_cam(const QString urlStream){
    ivbackend::StreamIdentifier streamIdentifier;
    streamIdentifier.set_url(urlStream.toStdString());

    QFuture<void> future = QtConcurrent::run([this, streamIdentifier] {
        m_backend->StartIncomingStream(streamIdentifier);
    });

    QFutureWatcher<void> watcher;
    watcher.setFuture(future);

    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(m_wait_time); 

    QEventLoop loop;
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit); 
    QObject::connect(&watcher, &QFutureWatcher<void>::finished, &loop, &QEventLoop::quit); 

    timer.start();
    loop.exec();

    if (watcher.isFinished()) {
        return false; 
    } else {
        return true; 
    }
}


void GRPCWriter::stop_cam(const QString urlStream){
    ivbackend::StreamIdentifier streamIdentifier;
    streamIdentifier.set_url(urlStream.toStdString());
    m_backend->StopIncomingStream(streamIdentifier);
}



