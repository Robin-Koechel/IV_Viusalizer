#include "GrpcReader.h"
#include "../DataStreams/IvBackendStream.h"
#include <QtConcurrent/QtConcurrent>
#include <QString>

GRPCReader::GRPCReader(QString url_grpc) : Reader(url_grpc)
{
    m_backend = new IvBackendClient();
    m_backend->SetServerAddress(url_grpc.toStdString());
}

DataStream* GRPCReader::read_images(const QString urlStream,TimeRange& timeRange, const ViewTypeEnum& viewType){
     IvBackendClient* backend = new IvBackendClient();
    backend->SetServerAddress(m_url.toStdString());
    IvBackendStream* stream = new IvBackendStream(backend);
    ivbackend::ImageRequest request;
    ivbackend::StreamIdentifier* streamIdentifier = new ivbackend::StreamIdentifier();
    streamIdentifier->set_url(urlStream.toStdString());
    request.set_allocated_identifier(streamIdentifier);
    ivbackend::TimeRange* time = new ivbackend::TimeRange(MessageConverter::createIvbackendTimeRange(timeRange));
    request.set_allocated_range(time);
    request.set_viewtype(MessageConverter::convertToIvbackendViewType(viewType));

    auto job = [backend, request = std::move(request), stream]() { 
        bool image_result = backend->GetImages(request, 
                [stream](const ivbackend::DataStream& imageStream){
                    stream->add_data_frame(imageStream);
                }
            );
        if(image_result){
            stream->stop_stream();
        }
    };

auto future = QtConcurrent::run(job);
    return stream;
}
Config* GRPCReader::read_config(const QString urlStream){
    ivbackend::StreamIdentifier streamIdentifier;
    streamIdentifier.set_url(urlStream.toStdString());
    ivbackend::ConfigResponseMessage response = m_backend->GetConfig(streamIdentifier);
    Config* con = new Config(MessageConverter::createConfigOutOfMessage(response));
    return con;
}
DataStream* GRPCReader::read_live_images(const QString urlStream, const ViewTypeEnum& viewType){
   IvBackendClient* backend = new IvBackendClient();
    backend->SetServerAddress(m_url.toStdString());
    IvBackendStream* stream = new IvBackendStream(backend);
    ivbackend::ImageRequest request;
    ivbackend::StreamIdentifier* streamIdentifier = new ivbackend::StreamIdentifier();
    streamIdentifier->set_url(urlStream.toStdString());
    
    request.set_allocated_identifier(streamIdentifier);
    TimeRange range(0,0);
    ivbackend::TimeRange* time = new ivbackend::TimeRange(MessageConverter::createIvbackendTimeRange(range));
    request.set_allocated_range(time);
    request.set_viewtype(MessageConverter::convertToIvbackendViewType(viewType));
    auto job = [backend, request = std::move(request), stream](){
        bool image_result = backend->GetLiveImages(request, 
            [stream](const ivbackend::DataStream& imageStream){
                stream->add_data_frame(imageStream);
            }
        );
         if(image_result){
            stream->stop_stream();
        }
    };
    auto future = QtConcurrent::run(job);
    return stream;

}
QVector<QVector<TimeRange*>>* GRPCReader::read_range_info(const QString urlStream){
    ivbackend::StreamIdentifier streamIdentifier;
    streamIdentifier.set_url(urlStream.toStdString());
    ivbackend::RangeInfo range_info = m_backend->GetRange(streamIdentifier);
    return MessageConverter::createTimeRanges(range_info);
}

QVector<QString>* GRPCReader::read_available_urls(){
    ivbackend::StreamIdentifierList url_list = m_backend->GetAvailableUrls();
    return MessageConverter::createListOfStreamIdentifier(url_list);
}
