#include "MessageConverter.h"

std::shared_ptr<Image> MessageConverter::convertDataStreamToImage(const ivcontainerpipeline::ImageData& imageData, uint64_t timestamp) {
    const std::string& dataStreamString = imageData.data();
    const uchar* data = reinterpret_cast<const uchar*>(dataStreamString.data());
    int width = imageData.meta_data().width();
    int height = imageData.meta_data().height();

    return std::make_shared<Image>(data, width, height, timestamp);
}

std::shared_ptr<DataFrame> MessageConverter::ConvertDataStreamToDataFrame(const ivcontainerpipeline::DataStream& dataStream, const int& fps, const QString& url) {
    auto dataFrame = std::make_shared<DataFrame>();

    // Convert ImageData to Image
    const ivcontainerpipeline::ImageData& imageData = dataStream.data().image();
    auto image = convertDataStreamToImage(imageData, dataStream.data().timestamp());
    dataFrame->setImage(image);

    // Convert BoundingBoxList to BoundingBoxes
    QVector<std::shared_ptr<iv_annotation::BoundingBox>> boundingBoxes;
const ivcontainerpipeline::BoundingBoxList& boundingBoxList = dataStream.data().boundingboxes();
for (const ivcontainerpipeline::BoundingBoxData& bboxData : boundingBoxList.items()) {
    // Convert BoundingBoxData to BoundingBox and append pointer to the QVector
auto size = cv::Size2i(image->width(), image->height());
std::vector<int> coords = {bboxData.x1(), bboxData.y1(), bboxData.x2(), bboxData.y2()};

auto boundingBox = std::make_shared<iv_annotation::BoundingBox>(
    coords,
    bboxData.confidence(),
    QString::fromStdString(bboxData.label()),
    size,
    iv_annotation::BoundingBox::Format::TLWH,
    0
);



    boundingBoxes.append(boundingBox);
}
dataFrame->setBoundingBoxen(boundingBoxes);

    // Convert RoiList to RegionsOfInterest
QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest;
const ivcontainerpipeline::RoiList& roiList = dataStream.data().roi_list();
for (const ivcontainerpipeline::Roi& roi : roiList.items()) {
    QVector<QPoint> polygonPoints;
    
    // Iterate over the points and convert them
    for (const ivcontainerpipeline::Point& point : roi.polygon()) {
        polygonPoints.append(QPoint(point.x(), point.y()));
    }

    // Convert Roi to RegionOfInterest
    auto regionOfInterest = std::make_shared<iv_annotation::RegionOfInterest>(
        QString::fromStdString(roi.name()), roi.id(),
        polygonPoints, roi.type() == ivcontainerpipeline::RoiType::INSIDE,
        100.0, QColor(255, 255, 255));
    regionsOfInterest.append(regionOfInterest);
}
dataFrame->setRegionsOfInterest(regionsOfInterest);

dataFrame->setUrl(url);

dataFrame->setFps(fps);

return dataFrame;
}
// Needed? v

ivcontainerpipeline::ConfigContainer MessageConverter::ConvertContainerFromBackendToPipeline(const ivbackend::ConfigContainer& container){
    ivcontainerpipeline::ConfigContainer newContainer;
    newContainer.set_log_info(container.log_info());
    newContainer.set_log_debug(container.log_debug());
    newContainer.set_jpg_image_quality(container.jpg_image_quality());
    return newContainer;
}

ivcontainerpipeline::InferenceType MessageConverter::ConvertInferenceTypeFromBackendToPipeline(const ivbackend::InferenceType& type){
    ivcontainerpipeline::InferenceType newType;
    switch(type) {
        case ivbackend::InferenceType::ONNX_CPU:
            return ivcontainerpipeline::InferenceType::ONNX_CPU;
        case ivbackend::InferenceType::ONNX_GPU:
            return ivcontainerpipeline::InferenceType::ONNX_GPU;
        case ivbackend::InferenceType::TRT_FLOAT16:
            return ivcontainerpipeline::InferenceType::TRT_FLOAT16;
        case ivbackend::InferenceType::TRT_FLOAT32:
            return ivcontainerpipeline::InferenceType::TRT_FLOAT32;
        default:
            std::cerr << "Unkown InferenceType" << std::endl;
            return ivcontainerpipeline::InferenceType::ONNX_CPU;
    }
}

ivcontainerpipeline::RoiType MessageConverter::ConvertRoiTypeFromBackendToPipeline(const ivbackend::RoiType & type){
    ivcontainerpipeline::RoiType newDetector;
    if(type == ivbackend::RoiType::INSIDE){
        return ivcontainerpipeline::RoiType::INSIDE;
    }
    else{
        return ivcontainerpipeline::RoiType::OUTSIDE;
    }
}

ivcontainerpipeline::ConfigDetector MessageConverter::ConvertDetectorFromBackendToPipeline(const ivbackend::ConfigDetector& detector){
    ivcontainerpipeline::ConfigDetector newDetector;
    newDetector.set_inference_type(ConvertInferenceTypeFromBackendToPipeline(detector.inference_type()));
    newDetector.set_cuda_device(detector.cuda_device());
    newDetector.set_nms_threshold(detector.nms_threshold());
    newDetector.set_confidence_threshold(detector.confidence_threshold());
    ivcontainerpipeline::RoiList roiList;
     for (const auto& roi : detector.rois().items()) {
        auto* newRoi = roiList.add_items();
        newRoi->set_name(roi.name());
        newRoi->set_id(roi.id());
        newRoi->set_type(ConvertRoiTypeFromBackendToPipeline(roi.type()));
        for (const auto& point : roi.polygon()) {
            auto* newPoint = newRoi->add_polygon(); 
            newPoint->set_x(point.x());
            newPoint->set_y(point.y());
        }
    }
    newDetector.mutable_rois()->CopyFrom(roiList);
    return newDetector;
}

ivcontainerpipeline::ConfigGStreamer MessageConverter::ConvertGStreamerFromBackendToPipeline(const ivbackend::ConfigGStreamer & gStreamer){
    ivcontainerpipeline::ConfigGStreamer newGStreamer;
    newGStreamer.set_stream_url(gStreamer.stream_url().url());
    newGStreamer.set_target_fps(gStreamer.target_fps ());
    newGStreamer.set_custom_size_width(gStreamer.custom_size_width());
    newGStreamer.set_custom_size_height(gStreamer.custom_size_height());
    return newGStreamer;
}

ivcontainerpipeline::SetConfig MessageConverter::ConvertToIvContainerPipelineSetConfig(const ivbackend::ConfigResponseMessage& sourceConfig) {
    ivcontainerpipeline::SetConfig setConfig;

      // Copy ConfigContainer
    setConfig.mutable_config_container()->CopyFrom(ConvertContainerFromBackendToPipeline(sourceConfig.config_container()));

    // Copy ConfigDetector
    setConfig.mutable_config_detector()->CopyFrom(ConvertDetectorFromBackendToPipeline(sourceConfig.config_detector()));

    // Copy ConfigGStreamer
    setConfig.mutable_config_gstreamer()->CopyFrom(ConvertGStreamerFromBackendToPipeline(sourceConfig.config_gstreamer()));


    return setConfig;
}

void MessageConverter::PopulateConfigResponse(ivbackend::ConfigResponseMessage* receivedConfig, const ivbackend::ConfigResponseMessage* request) {
    receivedConfig->mutable_config_container()->set_log_info(request->config_container().log_info());
    receivedConfig->mutable_config_container()->set_log_debug(request->config_container().log_debug());
    receivedConfig->mutable_config_container()->set_jpg_image_quality(request->config_container().jpg_image_quality());

    receivedConfig->mutable_config_detector()->set_inference_type(request->config_detector().inference_type());
    receivedConfig->mutable_config_detector()->set_cuda_device(request->config_detector().cuda_device());
    receivedConfig->mutable_config_detector()->set_nms_threshold(request->config_detector().nms_threshold());
    receivedConfig->mutable_config_detector()->set_confidence_threshold(request->config_detector().confidence_threshold());

    receivedConfig->mutable_config_gstreamer()->mutable_stream_url()->set_url(request->config_gstreamer().stream_url().url());
    receivedConfig->mutable_config_gstreamer()->set_target_fps(request->config_gstreamer().target_fps());
    receivedConfig->mutable_config_gstreamer()->set_custom_size_width(request->config_gstreamer().custom_size_width());
    receivedConfig->mutable_config_gstreamer()->set_custom_size_height(request->config_gstreamer().custom_size_height());
}

// Converts ivcontainerpipeline datastream to ivbackend datastream
ivbackend::DataStream MessageConverter::ConvertToDataStream(const ivcontainerpipeline::DataStream& srcDataStream) {
    ivbackend::DataStream destDataStream;

    // Copying InfoMessage

        destDataStream.mutable_info()->set_info(srcDataStream.info().info());


        // Copying BoundingBoxList
        auto destBoundingBoxList = ConvertToBackendBoundingBoxList(srcDataStream.data().boundingboxes());
        destDataStream.mutable_data()->mutable_boundingboxes()->CopyFrom(destBoundingBoxList);

        // Copying ImageData
        destDataStream.mutable_data()->mutable_image()->set_data(srcDataStream.data().image().data());

        // Copying Metadata
        destDataStream.mutable_data()->mutable_image()->mutable_meta_data()->set_width(srcDataStream.data().image().meta_data().width());
        destDataStream.mutable_data()->mutable_image()->mutable_meta_data()->set_height(srcDataStream.data().image().meta_data().height());
        destDataStream.mutable_data()->mutable_image()->mutable_meta_data()->set_data_len(srcDataStream.data().image().meta_data().data_len());

        // Copying Timestamp
        destDataStream.mutable_data()->set_timestamp(srcDataStream.data().timestamp());

        // Copying RoiList
        auto destRoiList = ConvertToBackendRoiList(srcDataStream.data().roi_list());
        destDataStream.mutable_data()->mutable_roi_list()->CopyFrom(destRoiList);

    return destDataStream;
}




// // Todo replace
// ivbackend::ConfigResponseMessage MessageConverter::ConvertToBackendConfigResponse(const ivcontainerpipeline::ConfigResponseMessage& sourceConfig) {
//     ivbackend::ConfigResponseMessage backendConfigResponse;

//     // Copy ConfigContainer
//     backendConfigResponse.mutable_config_container()->CopyFrom(sourceConfig.config_container());

//     // Copy ConfigDetector
//     backendConfigResponse.mutable_config_detector()->CopyFrom(sourceConfig.config_detector());

//     // Copy ConfigGStreamer
//     backendConfigResponse.mutable_config_gstreamer()->CopyFrom(sourceConfig.config_gstreamer());

//     return backendConfigResponse;
// }

std::shared_ptr<iv_annotation::BoundingBox> MessageConverter::ConvertContainerBoundingBoxToAnnotationBoundingBox(const ivcontainerpipeline::BoundingBoxData& bounding_box) {
    int x1 = static_cast<int>(std::round(bounding_box.x1()));
    int y1 = static_cast<int>(std::round(bounding_box.y1()));
    int x2 = static_cast<int>(std::round(bounding_box.x2()));
    int y2 = static_cast<int>(std::round(bounding_box.y2()));

    int width = x2 - x1;
    int height = y2 - y1;

    return std::make_shared<iv_annotation::BoundingBox>(
        iv_annotation::BoundingBox({x1, y1, width, height},
        bounding_box.confidence(),
        QString::fromStdString(bounding_box.label()),
        cv::Size2i(width, height),
        iv_annotation::BoundingBox::Format::TLWH,
        0) // Set appropriate rotation
    );
}

std::shared_ptr<iv_annotation::RegionOfInterest> MessageConverter::ConvertContainerRegionOfInterestToAnnotationRegionOfInterest(const ivcontainerpipeline::Roi& roi) {
    QString name = QString::fromStdString(roi.name());
    int id = roi.id();

    QPolygon polygon;
    for (const auto& point : roi.polygon()) {
        polygon << QPoint(point.x(), point.y());
    }

    bool is_inside = (roi.type() == ivcontainerpipeline::RoiType::INSIDE);
    float opacity = 100.0; 
    QColor color(255, 255, 255); 

    return std::make_shared<iv_annotation::RegionOfInterest>(
        iv_annotation::RegionOfInterest(name, id, polygon, is_inside, opacity, color)
    );
}


ivbackend::TimeRange MessageConverter::convertToProtoTimeRange(const TimeRange& timeRange) {
    ivbackend::TimeRange protoTimeRange;
    protoTimeRange.set_start(timeRange.getStartTime());
    protoTimeRange.set_end(timeRange.getEndTime());
    protoTimeRange.set_fps(timeRange.getFps());
    return protoTimeRange;
}

// Function to convert ivbackend::ConfigReponseMessage to ivcontainerpipeline::ConfigResponseMessage
ivcontainerpipeline::ConfigResponseMessage MessageConverter::ConvertToContainerConfigResponse(const ivbackend::ConfigResponseMessage& backendConfig) {
    ivcontainerpipeline::ConfigResponseMessage containerConfig;

    // Copy ConfigContainer
    containerConfig.mutable_config_container()->set_log_info(backendConfig.config_container().log_info());
    containerConfig.mutable_config_container()->set_log_debug(backendConfig.config_container().log_debug());
    containerConfig.mutable_config_container()->set_jpg_image_quality(backendConfig.config_container().jpg_image_quality());

    // Copy ConfigDetector
    auto* detectorConfig = containerConfig.mutable_config_detector();
    detectorConfig->set_inference_type(static_cast<ivcontainerpipeline::InferenceType>(backendConfig.config_detector().inference_type()));
    detectorConfig->set_cuda_device(backendConfig.config_detector().cuda_device());
    detectorConfig->set_nms_threshold(backendConfig.config_detector().nms_threshold());
    detectorConfig->set_confidence_threshold(backendConfig.config_detector().confidence_threshold());

    // Copy RoiList
    const auto& backendRois = backendConfig.config_detector().rois();
    auto* containerRois = detectorConfig->mutable_rois();
    for (const auto& backendRoi : backendRois.items()) {
        auto* containerRoi = containerRois->add_items();
        containerRoi->set_name(backendRoi.name());
        containerRoi->set_id(backendRoi.id());
        containerRoi->set_type(static_cast<ivcontainerpipeline::RoiType>(backendRoi.type()));
        for (const auto& point : backendRoi.polygon()) {
            auto* containerPoint = containerRoi->add_polygon();
            containerPoint->set_x(point.x());
            containerPoint->set_y(point.y());
        }
    }

    // Copy ConfigGStreamer
    ivcontainerpipeline::ConfigGStreamer* gstreamerConfig = containerConfig.mutable_config_gstreamer();
    gstreamerConfig->set_stream_url(backendConfig.config_gstreamer().stream_url().url());
    gstreamerConfig->set_target_fps(backendConfig.config_gstreamer().target_fps());
    gstreamerConfig->set_custom_size_width(backendConfig.config_gstreamer().custom_size_width());
    gstreamerConfig->set_custom_size_height(backendConfig.config_gstreamer().custom_size_height());

    return containerConfig;
}

// Function to convert ivcontainerpipeline::ConfigResponseMessage to ivbackend::ConfigReponseMessage
ivbackend::ConfigResponseMessage MessageConverter::ConvertToBackendConfigResponse(const ivcontainerpipeline::ConfigResponseMessage& containerConfig) {
    ivbackend::ConfigResponseMessage backendConfig;

    // Copy ConfigContainer
    backendConfig.mutable_config_container()->set_log_info(containerConfig.config_container().log_info());
    backendConfig.mutable_config_container()->set_log_debug(containerConfig.config_container().log_debug());
    backendConfig.mutable_config_container()->set_jpg_image_quality(containerConfig.config_container().jpg_image_quality());

    // Copy ConfigDetector
    auto* backendDetectorConfig = backendConfig.mutable_config_detector();
    backendDetectorConfig->set_inference_type(static_cast<ivbackend::InferenceType>(containerConfig.config_detector().inference_type()));
    backendDetectorConfig->set_cuda_device(containerConfig.config_detector().cuda_device());
    backendDetectorConfig->set_nms_threshold(containerConfig.config_detector().nms_threshold());
    backendDetectorConfig->set_confidence_threshold(containerConfig.config_detector().confidence_threshold());

    // Copy RoiList
    const auto& containerRois = containerConfig.config_detector().rois();
    auto* backendRois = backendDetectorConfig->mutable_rois();
    for (const auto& containerRoi : containerRois.items()) {
        auto* backendRoi = backendRois->add_items();
        backendRoi->set_name(containerRoi.name());
        backendRoi->set_id(containerRoi.id());
        backendRoi->set_type(static_cast<ivbackend::RoiType>(containerRoi.type()));
        for (const auto& point : containerRoi.polygon()) {
            auto* backendPoint = backendRoi->add_polygon();
            backendPoint->set_x(point.x());
            backendPoint->set_y(point.y());
        }
    }

    // Copy ConfigGStreamer
    ivbackend::ConfigGStreamer* backendGstreamerConfig = backendConfig.mutable_config_gstreamer();
    const auto& containerGstreamerConfig = containerConfig.config_gstreamer();
    backendGstreamerConfig->mutable_stream_url()->set_url(containerGstreamerConfig.stream_url());
    backendGstreamerConfig->set_target_fps(containerGstreamerConfig.target_fps());
    backendGstreamerConfig->set_custom_size_width(containerGstreamerConfig.custom_size_width());
    backendGstreamerConfig->set_custom_size_height(containerGstreamerConfig.custom_size_height());

    return backendConfig;
}

ivbackend::DataStream MessageConverter::ConvertDataFrameToDataStream(DataFrame& dataFrame) {
    ivbackend::DataStream dataStream;

    // Convert Image to ImageData
    std::shared_ptr<Image> image = dataFrame.getImage();
    if (image) {
        // Use the convertImageToImageData function to convert Image to ImageData
        // TODO, use shared pointer?
        std::shared_ptr<ivbackend::ImageData> imageData = convertImageToImageData(image);

        // Set the allocated image in dataStream
        //dataStream.mutable_data()->set_allocated_image(imageData.get());
        dataStream.mutable_data()->mutable_image()->CopyFrom(*imageData);
    }

    // Convert BoundingBoxen to BoundingBoxList
    const QVector<std::shared_ptr<iv_annotation::BoundingBox>>& boundingBoxen = dataFrame.getBoundingBoxen();
    for (const auto& boundingBox : boundingBoxen) {
        // Convert BoundingBox to BoundingBoxData and add to BoundingBoxList
        ivbackend::BoundingBoxData* bboxData = dataStream.mutable_data()->mutable_boundingboxes()->add_items();

        std::vector<int> bboxCoords = boundingBox->get_coords(iv_annotation::BoundingBox::Format::TLWH);
        int x1 = bboxCoords[0];
        int y1 = bboxCoords[1];
        int x2 = bboxCoords[2];
        int y2 = bboxCoords[3];

        bboxData->set_x1(x1);
        bboxData->set_y1(y1);
        bboxData->set_x2(x2);
        bboxData->set_y2(y2);
        bboxData->set_confidence(boundingBox->get_confidence());
        bboxData->set_label(boundingBox->get_label().toStdString());
    }

    // Convert RegionsOfInterest to RoiList
    const QVector<std::shared_ptr<iv_annotation::RegionOfInterest>>& regionsOfInterest = dataFrame.getRegionsOfInterest();
    for (const auto& regionOfInterest : regionsOfInterest) {
        // Convert RegionOfInterest to Roi and add to RoiList
        ivbackend::Roi* roi = dataStream.mutable_data()->mutable_roi_list()->add_items();
        roi->set_name(regionOfInterest->get_name().toStdString());
        roi->set_id(regionOfInterest->get_id());
        roi->set_type(static_cast<ivbackend::RoiType>(regionOfInterest->get_is_inside()));

        // Convert QVector<iv_annotation::Point> to repeated Point
        for (const QPoint& point : regionOfInterest->get_polygon()) {
            // Convert iv_annotation::Point to ivcontainerpipeline::Point
            ivbackend::Point* protoPoint = roi->add_polygon();
            protoPoint->set_x(point.x());
            protoPoint->set_y(point.y());
        }
    }

    // Set the remaining properties
    dataStream.mutable_data()->set_timestamp(dataFrame.getTimestamp());

    return dataStream;
}


std::shared_ptr<ivbackend::ImageData> MessageConverter::convertImageToImageData(const std::shared_ptr<Image> image) {
    auto imageData = std::make_shared<ivbackend::ImageData>();

    if (image) {
        const uchar* imageDataPtr = image->bits();
        int dataSize = image->sizeInBytes();
        std::string imageDataString(reinterpret_cast<const char*>(imageDataPtr), dataSize);

        // Set binary image data
        //QByteArray byteArray(reinterpret_cast<const char*>(image->constBits()), image->sizeInBytes());
        *imageData->mutable_data() = imageDataString;  
        // Set metadata
        ivbackend::Metadata* metaData = imageData->mutable_meta_data();
        metaData->set_width(image->width());
        metaData->set_height(image->height());
        metaData->set_data_len(dataSize);
    }

    return imageData;
}


ivbackend::BoundingBoxData MessageConverter::ConvertToBackendBoundingBox(const ivcontainerpipeline::BoundingBoxData& srcBoundingBox) {
    ivbackend::BoundingBoxData destBoundingBox;
    
    destBoundingBox.set_x1(srcBoundingBox.x1());
    destBoundingBox.set_y1(srcBoundingBox.y1());
    destBoundingBox.set_x2(srcBoundingBox.x2());
    destBoundingBox.set_y2(srcBoundingBox.y2());
    destBoundingBox.set_confidence(srcBoundingBox.confidence());
    destBoundingBox.set_label(srcBoundingBox.label());

    return destBoundingBox;
}

ivbackend::BoundingBoxList MessageConverter::ConvertToBackendBoundingBoxList(const ivcontainerpipeline::BoundingBoxList& srcBoundingBoxList) {
    ivbackend::BoundingBoxList destBoundingBoxList;

    for (const auto& srcBoundingBox : srcBoundingBoxList.items()) {
        auto destBoundingBox = ConvertToBackendBoundingBox(srcBoundingBox);
        destBoundingBoxList.add_items()->CopyFrom(destBoundingBox);
    }

    return destBoundingBoxList;
}

ivbackend::Point MessageConverter::ConvertToBackendPoint(const ivcontainerpipeline::Point& srcPoint) {
    ivbackend::Point destPoint;
    destPoint.set_x(srcPoint.x());
    destPoint.set_y(srcPoint.y());
    return destPoint;
}

ivbackend::Roi MessageConverter::ConvertToBackendRoi(const ivcontainerpipeline::Roi& srcRoi) {
    ivbackend::Roi destRoi;
    destRoi.set_name(srcRoi.name());
    destRoi.set_id(srcRoi.id());
    destRoi.set_type(static_cast<ivbackend::RoiType>(srcRoi.type()));

    for (const auto& srcPoint : srcRoi.polygon()) {
        auto destPoint = ConvertToBackendPoint(srcPoint);
        destRoi.add_polygon()->CopyFrom(destPoint);
    }

    return destRoi;
}

ivbackend::RoiList MessageConverter::ConvertToBackendRoiList(const ivcontainerpipeline::RoiList& srcRoiList) {
    ivbackend::RoiList destRoiList;

    for (const auto& srcRoi : srcRoiList.items()) {
        auto destRoi = ConvertToBackendRoi(srcRoi);
        destRoiList.add_items()->CopyFrom(destRoi);
    }

    return destRoiList;
}

QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> MessageConverter::extractRegionsOfInterestIvBackend(const ivbackend::ConfigResponseMessage& configResponse) {
    QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest;

    // Iterate through the RoiList in the ConfigDetector message and convert each Roi to iv_annotation::RegionOfInterest
    for (const auto& roi : configResponse.config_detector().rois().items()) {
        auto regionOfInterest = std::make_shared<iv_annotation::RegionOfInterest>();
        QString name = QString::fromStdString(roi.name());
        regionOfInterest->set_name(name);
        regionOfInterest->set_id(roi.id());
        regionOfInterest->set_is_inside(roi.type());
        
        // Convert each Point in the Roi to iv_annotation::Point
        QPolygon polygon;
        for (const auto& point : roi.polygon()) {
            QPoint polygonPoint(point.x(), point.y());
            polygon << polygonPoint;
        }
        regionOfInterest->set_polygon(polygon);

        regionsOfInterest.push_back(regionOfInterest);
    }

    return regionsOfInterest;
}

QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> MessageConverter::extractRegionsOfInterestIvContainer(const ivcontainerpipeline::ConfigResponseMessage& configResponse) {
    QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionsOfInterest;

    // Iterate through the RoiList in the ConfigDetector message and convert each Roi to iv_annotation::RegionOfInterest
    for (const auto& roi : configResponse.config_detector().rois().items()) {
        auto regionOfInterest = std::make_shared<iv_annotation::RegionOfInterest>();
        QString name = QString::fromStdString(roi.name());
        regionOfInterest->set_name(name);
        regionOfInterest->set_id(roi.id());
        regionOfInterest->set_is_inside(roi.type());
        
        // Convert each Point in the Roi to iv_annotation::Point
        QPolygon polygon;
        for (const auto& point : roi.polygon()) {
            QPoint polygonPoint(point.x(), point.y());
            polygon << polygonPoint;
        }
        regionOfInterest->set_polygon(polygon);

        regionsOfInterest.push_back(regionOfInterest);
    }

    return regionsOfInterest;
}
