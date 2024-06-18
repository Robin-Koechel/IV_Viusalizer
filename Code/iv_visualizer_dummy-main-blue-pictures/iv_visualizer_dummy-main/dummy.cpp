#include <iostream>
#include <string>
#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>
#include "ivcontainer.grpc.pb.h"
#include <QImage>
#include <QColor>
#include <thread>
#include <QPainter>
#include <QGuiApplication>

class DummyIvService final : public ivcontainerpipeline::IvService::Service {
public:
int currentFPS = 30;
    grpc::Status Init(grpc::ServerContext* context, const ivcontainerpipeline::Empty* request, ivcontainerpipeline::ConfigResponseMessage* response) override {
        *response = generateRandomConfig();
        return grpc::Status::OK;
    }

    grpc::Status Start(grpc::ServerContext* context, const ivcontainerpipeline::Empty* request, grpc::ServerWriter<ivcontainerpipeline::DataStream>* writer) override {
    while (true) {
        ivcontainerpipeline::ImageAndBoundingBox imageAndBBox = generateRandomData();
        ivcontainerpipeline::DataStream dataStream;
        dataStream.mutable_data()->CopyFrom(imageAndBBox);
        if (!writer->Write(dataStream)) {
            // The client has disconnected or encountered an error.
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/currentFPS));
        auto now = std::chrono::high_resolution_clock::now();
        auto epoch = now.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch);

        std::cout << value.count() << " ns" << std::endl;    
    }
        return grpc::Status::OK;
    }

    grpc::Status Stop(grpc::ServerContext* context, const ivcontainerpipeline::Empty* request, ivcontainerpipeline::InfoMessage* response) override {
        response->set_info("Stream stopped.");
        return grpc::Status::OK;
    }

    grpc::Status UpdateConfig(grpc::ServerContext* context, const ivcontainerpipeline::SetConfig* request, ivcontainerpipeline::ConfigResponseMessage* response) override {
        if (request->has_config_container()) {
            *response->mutable_config_container() = request->config_container();
        } else if (request->has_config_detector()) {
            *response->mutable_config_detector() = request->config_detector();
        } else if (request->has_config_gstreamer()) {
            *response->mutable_config_gstreamer() = request->config_gstreamer();
        } else {
        // Handle the case where none of the oneof fields are set
        return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "No valid configuration provided");
        }
        return grpc::Status::OK;
    }

    grpc::Status GetConfig(grpc::ServerContext* context, const ivcontainerpipeline::Empty* request, ivcontainerpipeline::ConfigResponseMessage* response) override {
        *response = generateRandomConfig();
        return grpc::Status::OK;
    }

private:
    ivcontainerpipeline::ConfigResponseMessage generateRandomConfig() {
        ivcontainerpipeline::ConfigResponseMessage config;
        config.mutable_config_container()->set_log_info(true);
        config.mutable_config_container()->set_log_debug(true);
        config.mutable_config_container()->set_jpg_image_quality(80);

        config.mutable_config_detector()->set_inference_type(ivcontainerpipeline::InferenceType::ONNX_CPU);
        config.mutable_config_detector()->set_cuda_device(0);
        config.mutable_config_detector()->set_nms_threshold(0.5);
        config.mutable_config_detector()->set_confidence_threshold(0.7);

        config.mutable_config_gstreamer()->set_stream_url("rtsp://user:pw@ip:port/media");
        config.mutable_config_gstreamer()->set_target_fps(currentFPS);
        config.mutable_config_gstreamer()->set_custom_size_width(640);
        config.mutable_config_gstreamer()->set_custom_size_height(480);

        return config;
    }

    ivcontainerpipeline::ImageAndBoundingBox generateRandomData() {
        ivcontainerpipeline::ImageAndBoundingBox imageAndBBox;
        ivcontainerpipeline::BoundingBoxData boundingBox;
        boundingBox.set_x1(100);
        boundingBox.set_y1(22);
        boundingBox.set_x2(300);
        boundingBox.set_y2(300);
        boundingBox.set_confidence(generateRandomFloat());
        boundingBox.set_label("Object");
        auto now = std::chrono::high_resolution_clock::now();
        auto epoch = now.time_since_epoch();
        auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch);
        ivcontainerpipeline::BoundingBoxList boundingBoxList;
        boundingBoxList.add_items()->CopyFrom(boundingBox);

        ivcontainerpipeline::Metadata metadata;
        QImage image(640, 480, QImage::Format_RGB888);

        image.fill(QColor(0, 0, 255));
        QPainter painter(&image);
        QString timestampString = QString::number(value.count());
        painter.drawText(image.rect(), Qt::AlignCenter, timestampString);
        painter.end();

        const uchar* imageDataPtr = image.bits();
        int dataSize = image.sizeInBytes(); 

        std::string imageDataString(reinterpret_cast<const char*>(imageDataPtr), dataSize);

        //ivcontainerpipeline::ImageData grpcImageData;
        //grpcImageData.set_data(imageDataString);

        metadata.set_width(image.width());
        metadata.set_height(image.height());
        metadata.set_data_len(dataSize);

        ivcontainerpipeline::ImageData imageData;
        imageData.set_data(imageDataString);
        imageData.mutable_meta_data()->CopyFrom(metadata);

        imageAndBBox.mutable_boundingboxes()->CopyFrom(boundingBoxList);
        imageAndBBox.mutable_image()->CopyFrom(imageData);
         
        imageAndBBox.set_timestamp(value.count());
        return imageAndBBox;
    }

    float generateRandomFloat() {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50052");
    DummyIvService service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    qputenv("QT_QPA_PLATFORM", QByteArray("offscreen"));

    int argc = 1;
    char* argv[] = {(char*)"dummy_app", nullptr};

    // Instanziieren einer QGuiApplication
    QGuiApplication app(argc, argv);

    RunServer();
    return 0;
}