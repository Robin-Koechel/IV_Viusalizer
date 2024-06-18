#include "MessageConverter.h"

// ================================================================================================
// FROM APPLICATION TO gRPC
// ================================================================================================

ivbackend::ImageAndAnnotation MessageConverter::convertDataFrameToImageAndAnnotation(const DataFrame& dataFrame) {
    ivbackend::ImageAndAnnotation imageAndAnnotation;

    // Convert Image to ImageData
    ivbackend::ImageData* imageData = imageAndAnnotation.mutable_image();
    convertImageToImageData(dataFrame.get_image(), *imageData);

    // Convert BoundingBoxen to BoundingBoxList
    ivbackend::BoundingBoxList* boundingBoxList = imageAndAnnotation.mutable_annotations();
    for (const BoundingBox* boundingBox : dataFrame.get_bounding_boxes()) {
        // Convert BoundingBox to BoundingBoxData
        ivbackend::BoundingBoxData* bboxData = boundingBoxList->add_items();
        convertBoundingBoxToBoundingBoxData(*boundingBox, *bboxData);
    }

    // Convert RegionsOfInterest to RegionOfInterestList
    ivbackend::RoiList* roiList = imageAndAnnotation.mutable_roi_list();
    for (const RegionOfInterest* regionOfInterest : dataFrame.get_regions_of_interest()) {
        // Convert RegionOfInterest to Roi
        ivbackend::Roi* roi = roiList->add_items();
        convertRegionOfInterestToRoi(*regionOfInterest, *roi);
    }

    return imageAndAnnotation;
}

void MessageConverter::convertImageToImageData(const Image& image, ivbackend::ImageData& imageData) {
    const uchar* imageDataPtr = image.bits();
    int imageWidth = image.width();
    int imageHeight = image.height();
    uint64_t timestamp = image.get_timestamp();

    // Set ImageData properties
    imageData.set_data(imageDataPtr, imageWidth * imageHeight * sizeof(QRgb));
    ivbackend::Metadata* meta_data = imageData.mutable_meta_data();
    meta_data->set_width(imageWidth);
    meta_data->set_height(imageHeight);
    meta_data->set_data_len(imageWidth * imageHeight * sizeof(QRgb));
}

void MessageConverter::convertBoundingBoxToBoundingBoxData(const BoundingBox& boundingBox, ivbackend::BoundingBoxData& bboxData) {
    std::vector<int> coords = boundingBox.get_coords(BoundingBox::Format::TLWH);

    // Set BoundingBoxData properties
    bboxData.set_x1(coords[0]);
    bboxData.set_y1(coords[1]);
    bboxData.set_x2(coords[0] + coords[2]);
    bboxData.set_y2(coords[1] + coords[3]);
    bboxData.set_confidence(boundingBox.get_confidence());
    bboxData.set_label(boundingBox.get_label().toStdString());
}

void MessageConverter::convertRegionOfInterestToRoi(const RegionOfInterest& regionOfInterest, ivbackend::Roi& roi) {
    QString name = regionOfInterest.get_name();
    int id = regionOfInterest.get_id();
    const QPolygon& polygon = regionOfInterest.get_polygon();
    ivbackend::RoiType roiType = regionOfInterest.is_inside() ? ivbackend::INSIDE : ivbackend::OUTSIDE;  // Assuming INSIDE and OUTSIDE match the enum values
    const QColor& color = regionOfInterest.get_color();
    float opacity = regionOfInterest.get_opacity();

    // Set Roi properties
    roi.set_name(name.toStdString());
    roi.set_id(id);
    roi.set_type(roiType);
    // TODO?
    //roi.set_opacity(opacity);

    // Set polygon points
    for (const QPoint& point : polygon) {
        ivbackend::Point* roiPoint = roi.add_polygon();
        roiPoint->set_x(point.x());
        roiPoint->set_y(point.y());
    }
}

// ================================================================================================
// FROM gRPC TO APPLICATION
// ================================================================================================

DataFrame MessageConverter::convertImageAndAnnotationToDataFrame(const ivbackend::ImageAndAnnotation& imageAndAnnotation) {
    
    // Extract timestamp from ImageAndAnnotation
    uint64_t timestamp = imageAndAnnotation.timestamp();
    
    // Convert ImageData to Image
    const ivbackend::ImageData& imageData = imageAndAnnotation.image();
    Image image = convertImageDataToImage(imageData, timestamp);
    image.bits();

    // Convert BoundingBoxList to BoundingBoxen
    QVector<BoundingBox*> boundingBoxen;
    const ivbackend::BoundingBoxList& boundingBoxList = imageAndAnnotation.annotations();
    for (const ivbackend::BoundingBoxData& bboxData : boundingBoxList.items()) {
        // Convert BoundingBoxData to BoundingBox
        BoundingBox* boundingBox = convertBoundingBoxDataToBoundingBox(bboxData, image.width(), image.height());
        boundingBoxen.append(boundingBox);
    }

    // Convert RegionOfInterestList to RegionsOfInterest
    QVector<RegionOfInterest*> regionsOfInterest;
    const ivbackend::RoiList& roiList = imageAndAnnotation.roi_list();
    for (const ivbackend::Roi& roi : roiList.items()) {
        // Convert Roi to RegionOfInterest
        RegionOfInterest regionOfInterest = convertRoiToRegionOfInterest(roi);
        // Add the RegionOfInterest to the list
        regionsOfInterest.append(new RegionOfInterest(regionOfInterest));
}

    // Set properties in DataFrame
    DataFrame dataFrame;
    dataFrame.set_image(image);
    dataFrame.set_bounding_boxes(boundingBoxen);
    dataFrame.set_regions_of_interest(regionsOfInterest.toVector());

    return dataFrame;
}

ivbackend::RoiList MessageConverter::convertRegionsToRoiList(QVector<RegionOfInterest*> regionsOfInterest){
    ivbackend::RoiList rois;
    for(RegionOfInterest* roi : regionsOfInterest){
        MessageConverter conv;
        ivbackend::Roi* roi_of_pipeline = rois.add_items();
        conv.convertRegionOfInterestToRoi(*roi,*roi_of_pipeline);
    }
    return rois;
}


BoundingBox* MessageConverter::convertBoundingBoxDataToBoundingBox(const ivbackend::BoundingBoxData& bboxData, int width, int height) {
   std::vector<int> coords = {
        static_cast<int>(bboxData.x1()),
        static_cast<int>(bboxData.y1()),
        static_cast<int>(bboxData.x2()),
        static_cast<int>(bboxData.y2())
    };


    BoundingBox::Format format = BoundingBox::Format::TLWH;
    BoundingBox* boundingBox = new BoundingBox(coords, bboxData.confidence(), QString::fromStdString(bboxData.label()),cv::Size2i(width, height), BoundingBox::Format::TLWH);

    // Shouldnt they be optional?
    boundingBox->set_confidence(bboxData.confidence());
    boundingBox->set_rotation(0);

    return boundingBox;
}

// This is a bit tricky
// Image(and Annotation) itself does not carry the timestamp
// But just ImageAndAnnotation does
// So either we remove the timestamp from our image, and only put a timestamp
// to DataFrame, or we pass the timestamp in as param 
// Second option should be okay since this method will be called
// from within convertImageAndAnnotationToDataFrame
Image MessageConverter::convertImageDataToImage(const ivbackend::ImageData& imageData, const uint64_t timestamp) {
    const std::string& imageDataString = imageData.data();
    const uchar* data = reinterpret_cast<const uchar*>(imageDataString.data());
    int width = imageData.meta_data().width();
    int height = imageData.meta_data().height();

    return Image(data, width, height, timestamp);
}

RegionOfInterest MessageConverter::convertRoiToRegionOfInterest(const ivbackend::Roi& roi) {
    RegionOfInterest regionOfInterest;
    regionOfInterest.set_name(QString::fromStdString(roi.name()));
    regionOfInterest.set_id(roi.id());
    regionOfInterest.set_is_inside(roi.type() == ivbackend::RoiType::INSIDE);
    // TODO
    regionOfInterest.set_opacity(1);

    // Fill the QPolygon with points from Roi
    QPolygon polygon;
    for (const ivbackend::Point& point : roi.polygon()) {
        polygon << QPoint(point.x(), point.y());
    }
    regionOfInterest.set_polygon(polygon);

    return regionOfInterest;
}


// ================================================================================================

ivbackend::ConfigContainer MessageConverter::createConfigContainer(bool logInfo, bool logDebug, int32_t jpgImageQuality) {
    ivbackend::ConfigContainer configContainer;
    configContainer.set_log_info(logInfo);
    configContainer.set_log_debug(logDebug);
    configContainer.set_jpg_image_quality(jpgImageQuality);
    return configContainer;
}

ivbackend::ConfigDetector MessageConverter::createConfigDetector(
    ivbackend::InferenceType inferenceType, int32_t cudaDevice, float nmsThreshold, float confidenceThreshold) {
    ivbackend::ConfigDetector configDetector;
    configDetector.set_inference_type(inferenceType);
    configDetector.set_cuda_device(cudaDevice);
    configDetector.set_nms_threshold(nmsThreshold);
    configDetector.set_confidence_threshold(confidenceThreshold);
    return configDetector;
}

ivbackend::ConfigGStreamer MessageConverter::createConfigGStreamer(
    const std::string& streamUrl, int32_t targetFps, int32_t customSizeWidth, int32_t customSizeHeight) {
    ivbackend::ConfigGStreamer configGStreamer;
    ivbackend::StreamIdentifier* streamIdentifier = configGStreamer.mutable_stream_url();
    streamIdentifier->set_url(streamUrl);
    configGStreamer.set_target_fps(targetFps);
    configGStreamer.set_custom_size_width(customSizeWidth);
    configGStreamer.set_custom_size_height(customSizeHeight);
    return configGStreamer;
}

ivbackend::ConfigResponseMessage MessageConverter::createConfigResponseMessage(
    const ivbackend::ConfigContainer& configContainer,
    const ivbackend::ConfigDetector& configDetector,
    const ivbackend::ConfigGStreamer& configGStreamer) {

    ivbackend::ConfigResponseMessage configResponse;
    configResponse.mutable_config_container()->CopyFrom(configContainer);
    configResponse.mutable_config_detector()->CopyFrom(configDetector);
    configResponse.mutable_config_gstreamer()->CopyFrom(configGStreamer);
    return configResponse;
}




ConfigDetector MessageConverter::createConfigDetectorOutOfMessage(ivbackend::ConfigDetector configDetector){
    ConfigDetector detector_class;
    detector_class.set_inference_type(convertFromIvbackendInferenceType(configDetector.inference_type()));
    detector_class.set_cuda_device(configDetector.cuda_device());
    detector_class.set_nms_threshold(configDetector.nms_threshold());
    detector_class.set_confidence_threshold(configDetector.confidence_threshold());
    QVector<RegionOfInterest*> rois;
    ivbackend::RoiList roilist = configDetector.rois();
    for (const ivbackend::Roi& roi : roilist.items()) {
        MessageConverter conv;
        RegionOfInterest* region = new RegionOfInterest(conv.convertRoiToRegionOfInterest(roi));
        rois.append(region);
    }
    detector_class.set_rois(rois);
    return detector_class;
}

    
ConfigGStreamer MessageConverter::createConfigGStreamerOutOfMessage(ivbackend::ConfigGStreamer configGStreamer){
    ConfigGStreamer gStreamer_class;
    ivbackend::StreamIdentifier streamIdentifier = configGStreamer.stream_url();
    gStreamer_class.set_stream_url(QString::fromStdString(streamIdentifier.url()));
    gStreamer_class.set_target_fps(configGStreamer.target_fps());
    gStreamer_class.set_custom_size_width(configGStreamer.custom_size_width());
    gStreamer_class.set_custom_size_height(configGStreamer.custom_size_height());
    return gStreamer_class;
}

Config MessageConverter::createConfigOutOfMessage(ivbackend::ConfigResponseMessage configMessage){
    Config config_class;
    config_class.set_config_gstreamer(createConfigGStreamerOutOfMessage(configMessage.config_gstreamer()));
    config_class.set_config_container(createConfigContainerOutOfMessage(configMessage.config_container()));
    config_class.set_config_detector(createConfigDetectorOutOfMessage(configMessage.config_detector()));
    return config_class;
}


ConfigContainer MessageConverter::createConfigContainerOutOfMessage(ivbackend::ConfigContainer configContainer){
    ConfigContainer container_class;
    container_class.set_log_info(configContainer.log_info());
    container_class.set_log_debug(configContainer.log_debug());
    container_class.set_jpg_image_quality(configContainer.jpg_image_quality());
    return container_class;
}


QVector<QVector<TimeRange*>>* MessageConverter::createTimeRanges(ivbackend::RangeInfo rangeInfo){
    QVector<QVector<TimeRange*>>* time_info = new QVector<QVector<TimeRange*>>();
    time_info->append(QVector<TimeRange*>());
    time_info->append(QVector<TimeRange*>());
    for (const ivbackend::TimeRange& time : rangeInfo.range_annotations().annotations()) {
        (*time_info)[0].append(new TimeRange(createTimeRange(time))); 
    }
     for (const ivbackend::TimeRange& time : rangeInfo.range_videos().videos()) {
        (*time_info)[1].append(new TimeRange(createTimeRange(time))); 
    }

    return time_info;
}
TimeRange MessageConverter::createTimeRange(ivbackend::TimeRange timeRange){
    TimeRange range;
    range.set_start_time(timeRange.start());
    range.set_time_end(timeRange.end());
    range.set_fps(timeRange.fps());
    return range;
}

QVector<QString>* MessageConverter::createListOfStreamIdentifier(ivbackend::StreamIdentifierList streamList){
    const google::protobuf::RepeatedPtrField<ivbackend::StreamIdentifier> urls = streamList.urls();
    QVector<QString>* url_list = new QVector<QString>();
    for (const ivbackend::StreamIdentifier& url : urls) {
        url_list->append(QString::fromStdString(url.url()));
    }
    return url_list;
}


ivbackend::TimeRange MessageConverter::createIvbackendTimeRange(TimeRange time){
    ivbackend::TimeRange timeiv;
    timeiv.set_start(time.get_start_time());
    timeiv.set_end(time.get_end_time());
    timeiv.set_fps(time.get_fps());
    return timeiv;
}

ivbackend::InferenceType MessageConverter::getInferenceOutOfEnum(InferenceType type){
      switch(type) {
        case ONNX_CPU:
            return ivbackend::InferenceType::ONNX_CPU;
        case ONNX_GPU:
            return ivbackend::InferenceType::ONNX_GPU;
        case TRT_FLOAT16:
            return ivbackend::InferenceType::TRT_FLOAT16;
        case TRT_FLOAT32:
            return ivbackend::InferenceType::TRT_FLOAT32;
        default:
            std::cerr << "Unkown InferenceType" << std::endl;
            return ivbackend::InferenceType::ONNX_CPU;
    }

}


InferenceType MessageConverter::convertFromIvbackendInferenceType(ivbackend::InferenceType type) {
    switch(type) {
        case ivbackend::InferenceType::ONNX_CPU:
            return ONNX_CPU;
        case ivbackend::InferenceType::ONNX_GPU:
            return ONNX_GPU;
        case ivbackend::InferenceType::TRT_FLOAT16:
            return TRT_FLOAT16;
        case ivbackend::InferenceType::TRT_FLOAT32:
            return TRT_FLOAT32;
        default:
            std::cerr << "Unbekannter ivbackend::InferenceType" << std::endl;
            return ONNX_CPU;
    }
}

ivbackend::ViewType MessageConverter::convertToIvbackendViewType(ViewTypeEnum viewType) {
    switch(viewType) {
        case ViewTypeEnum::ONLY_VIDEO:
            return ivbackend::ViewType::ONLY_VIDEO;
        case ViewTypeEnum::ANNOTATION_AND_VIDEO:
            return ivbackend::ViewType::ANNOTATION_AND_VIDEO;
        case ViewTypeEnum::ANNOTATION_WITHOUT_VIDEO:
            return ivbackend::ViewType::ANNOTATION_WITHOUT_VIDEO;
        default:
            std::cerr << "Unkown ViewTypeEnum" << std::endl;
            return ivbackend::ViewType::UNKNOWN_VIEW_TYPE;
    }
}

ViewTypeEnum MessageConverter::convertViewTypeFromIvbackend(ivbackend::ViewType viewType) {
    switch(viewType) {
        case ivbackend::ViewType::ONLY_VIDEO:
            return ViewTypeEnum::ONLY_VIDEO;
        case ivbackend::ViewType::ANNOTATION_AND_VIDEO:
            return ViewTypeEnum::ANNOTATION_AND_VIDEO;
        case ivbackend::ViewType::ANNOTATION_WITHOUT_VIDEO:
            return ViewTypeEnum::ANNOTATION_WITHOUT_VIDEO;
        default:
            std::cerr << "Unkown ivbackend::ViewType" << std::endl;
            return ViewTypeEnum::ANNOTATION_WITHOUT_VIDEO;

    }
}









// TODO 
// Conversion in other direciton


// TODO 
// Conversion to and from DATAFRAme