#include <grpcpp/grpcpp.h>
#include <grpcpp/server_context.h>

#include <QColor>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "ivcontainer.grpc.pb.h"

class DummyIvService final : public ivcontainerpipeline::IvService::Service {
 public:
  std::map<int, std::vector<ivcontainerpipeline::BoundingBoxData>>
      m_annotation_map;
  int currentFPS = 25;
  bool m_is_running = true;


  // https://github.com/dendorferpatrick/MOTChallengeEvalKit/blob/master/MOT/MOTVisualization.py
std::map<int, std::vector<ivcontainerpipeline::BoundingBoxData>>
load_annotations(const QString& a_file_path = "./MOT20-01/gt/gt.txt") {
  std::map<int, std::vector<ivcontainerpipeline::BoundingBoxData>> output;
  QFile file(a_file_path);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::cerr << "Cannot open file for reading: " << a_file_path.toStdString()
              << std::endl;
    return output;
  }

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList parts = line.split(",", Qt::SkipEmptyParts);
    if (parts.size() != 9) {
      std::cout << "Invalid line format: " << line.toStdString() << std::endl;
    }

    int frame_num = parts[0].toInt();

    ivcontainerpipeline::BoundingBoxData boundingBox;
    boundingBox.set_y1(parts[3].toInt());  // Top is in index 3
    boundingBox.set_x1(parts[2].toInt());  // Left is in index 2
    boundingBox.set_x2(parts[4].toInt());
    boundingBox.set_y2(parts[5].toInt());
    boundingBox.set_confidence(parts[8].toFloat());
    boundingBox.set_label("Object");
   

    output[frame_num].push_back(boundingBox);
  }

  return output;
}


  grpc::Status Init(
      grpc::ServerContext* context, const ivcontainerpipeline::Empty* request,
      ivcontainerpipeline::ConfigResponseMessage* response) override {
    *response = generateRandomConfig();
    return grpc::Status::OK;
  }

  grpc::Status Start(
      grpc::ServerContext* context, const ivcontainerpipeline::Empty* request,
      grpc::ServerWriter<ivcontainerpipeline::DataStream>* writer) override {
    int max_frames = count_jpg_files();
    int frame_count = 1;
    m_annotation_map = load_annotations();
    auto start = std::chrono::high_resolution_clock::now();
    m_is_running = true;
    while (frame_count <= max_frames && m_is_running) {
      ivcontainerpipeline::ImageAndBoundingBox imageAndBBox = get_data_from_frame(frame_count);
      ivcontainerpipeline::DataStream dataStream;
      dataStream.mutable_data()->CopyFrom(imageAndBBox);
      
      if (!writer->Write(dataStream)) {
          break;
      }
      
      auto end = std::chrono::high_resolution_clock::now();
      std::chrono::milliseconds sleepDuration = std::chrono::milliseconds(1000 / currentFPS) - std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
      
      if(sleepDuration.count() > 0) {
          std::this_thread::sleep_for(sleepDuration);
      }
      
      start = std::chrono::high_resolution_clock::now(); 

      auto now = std::chrono::high_resolution_clock::now();
      auto epoch = now.time_since_epoch();
      auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch);
      std::cout << value.count() << " ns" << std::endl;
      frame_count = (frame_count % max_frames)+1;
    }
    return grpc::Status::OK;
  }

  grpc::Status Stop(grpc::ServerContext* context,
                    const ivcontainerpipeline::Empty* request,
                    ivcontainerpipeline::InfoMessage* response) override {
    response->set_info("Stream stopped.");
    m_is_running = false;
    return grpc::Status::OK;
  }

  grpc::Status UpdateConfig(
      grpc::ServerContext* context,
      const ivcontainerpipeline::SetConfig* request,
      ivcontainerpipeline::ConfigResponseMessage* response) override {
    if (request->has_config_container()) {
      *response->mutable_config_container() = request->config_container();
    } else if (request->has_config_detector()) {
      *response->mutable_config_detector() = request->config_detector();
    } else if (request->has_config_gstreamer()) {
      *response->mutable_config_gstreamer() = request->config_gstreamer();
    } else {
      // Handle the case where none of the oneof fields are set
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT,
                          "No valid configuration provided");
    }
    return grpc::Status::OK;
  }

  grpc::Status GetConfig(
      grpc::ServerContext* context, const ivcontainerpipeline::Empty* request,
      ivcontainerpipeline::ConfigResponseMessage* response) override {
    *response = generateRandomConfig();
    return grpc::Status::OK;
  }

 private:
  ivcontainerpipeline::ConfigResponseMessage generateRandomConfig() {
    ivcontainerpipeline::ConfigResponseMessage config;
    config.mutable_config_container()->set_log_info(true);
    config.mutable_config_container()->set_log_debug(true);
    config.mutable_config_container()->set_jpg_image_quality(80);

    config.mutable_config_detector()->set_inference_type(
        ivcontainerpipeline::InferenceType::ONNX_CPU);
    config.mutable_config_detector()->set_cuda_device(0);
    config.mutable_config_detector()->set_nms_threshold(0.5);
    config.mutable_config_detector()->set_confidence_threshold(0.7);

    config.mutable_config_gstreamer()->set_stream_url(
        "rtsp://user:pw@ip:port/media");
    config.mutable_config_gstreamer()->set_target_fps(currentFPS);
    config.mutable_config_gstreamer()->set_custom_size_width(640);
    config.mutable_config_gstreamer()->set_custom_size_height(480);

    return config;
  }

  int count_jpg_files(const QString& a_folder_path = "./MOT20-01/img1") {
    QDir dir(a_folder_path);

    QStringList filters;
    filters << "*.jpg";

    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);

    QStringList file_list = dir.entryList();

    return file_list.count();
  }

  QImage load_image(int a_frame_num,
                    const QString& a_folder_path = "./MOT20-01/img1") {
    QString filename = QString("%1.jpg").arg(a_frame_num, 6, 10, QChar('0'));

    QString image_path = QDir(a_folder_path).filePath(filename);
    QImage image(image_path);
    image = image.convertToFormat(QImage::Format_RGB888);
    if (image.isNull()) {
      std::cerr << "Failed to load image: " << image_path.toStdString()
                << std::endl;
      return QImage();
    }

    return image;
  }

  ivcontainerpipeline::ImageAndBoundingBox get_data_from_frame(
      const int a_frame_num) {
    ivcontainerpipeline::ImageAndBoundingBox imageAndBBox;

    auto now = std::chrono::high_resolution_clock::now();
    auto epoch = now.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::nanoseconds>(epoch);

    if (m_annotation_map.find(a_frame_num) != m_annotation_map.end()) {
      ivcontainerpipeline::BoundingBoxList boundingBoxList;
      for (auto& bbox : m_annotation_map[a_frame_num]) {
        boundingBoxList.add_items()->CopyFrom(bbox);
      }

      imageAndBBox.mutable_boundingboxes()->CopyFrom(boundingBoxList);
    }

    QString timestampString = QString::number(value.count());

    QImage image = load_image(a_frame_num);

    ivcontainerpipeline::Metadata metadata;
    const uchar* imageDataPtr = image.bits();
    int dataSize = image.sizeInBytes();

    std::string imageDataString(reinterpret_cast<const char*>(imageDataPtr),
                                dataSize);

    metadata.set_width(image.width());
    metadata.set_height(image.height());
    metadata.set_data_len(dataSize);

    ivcontainerpipeline::ImageData imageData;
    imageData.set_data(imageDataString);
    imageData.mutable_meta_data()->CopyFrom(metadata);

    imageAndBBox.mutable_image()->CopyFrom(imageData);

    imageAndBBox.set_timestamp(value.count());
    return imageAndBBox;
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

    std::string imageDataString(reinterpret_cast<const char*>(imageDataPtr),
                                dataSize);

    // ivcontainerpipeline::ImageData grpcImageData;
    // grpcImageData.set_data(imageDataString);

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

    builder.SetMaxReceiveMessageSize(-1);

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