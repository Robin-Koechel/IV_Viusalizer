#ifndef MESSAGE_CONVERTER_H
#define MESSAGE_CONVERTER_H

#include <grpcpp/grpcpp.h>
#include "ivbackend.grpc.pb.h"
#include "../FrameData/Data/RegionOfInterest.h"
#include "../FrameData/Data/Image.h"
#include "../FrameData/Data/BoundingBox.h"
#include "../FrameData/Data/DataFrame.h"
#include "TimeRange.h"
#include "../FrameData/Config/Config.h"
#include "../ViewType/ViewTypeEnum.h"
/**
 * @brief A utility class for converting basic data types to gRPC message types.
 */
class MessageConverter {
public:
    /**
     * @brief Creates a ConfigContainer message.
     * @param logInfo Whether to log information.
     * @param logDebug Whether to log debug messages.
     * @param jpgImageQuality The quality factor for JPEG images.
     * @return The ConfigContainer message.
     */
    static ivbackend::ConfigContainer createConfigContainer(bool logInfo, bool logDebug, int32_t jpgImageQuality);

    /**
     * @brief Creates a ConfigDetector message.
     * @param inferenceType The type of inference.
     * @param cudaDevice The CUDA device for GPU inference.
     * @param nmsThreshold The non-maximum suppression threshold.
     * @param confidenceThreshold The confidence threshold for detections.
     * @return The ConfigDetector message.
     */
    static ivbackend::ConfigDetector createConfigDetector(
        ivbackend::InferenceType inferenceType, int32_t cudaDevice, float nmsThreshold, float confidenceThreshold);

    /**
     * @brief Creates a ConfigGStreamer message.
     * @param streamUrl The URL of the GStreamer stream.
     * @param targetFps The target frames per second.
     * @param customSizeWidth The custom width for the video stream.
     * @param customSizeHeight The custom height for the video stream.
     * @return The ConfigGStreamer message.
     */
    static ivbackend::ConfigGStreamer createConfigGStreamer(
        const std::string& streamUrl, int32_t targetFps, int32_t customSizeWidth, int32_t customSizeHeight);

    /**
     * @brief Creates a ConfigResponseMessage from ConfigContainer, ConfigDetector, and ConfigGStreamer.
     * @param configContainer The ConfigContainer message.
     * @param configDetector The ConfigDetector message.
     * @param configGStreamer The ConfigGStreamer message.
     * @return The ConfigResponseMessage.
     */
    static ivbackend::ConfigResponseMessage createConfigResponseMessage(
        const ivbackend::ConfigContainer& configContainer,
        const ivbackend::ConfigDetector& configDetector,
        const ivbackend::ConfigGStreamer& configGStreamer);

    RegionOfInterest convertRoiToRegionOfInterest(const ivbackend::Roi& roi);

    Image convertImageDataToImage(const ivbackend::ImageData& imageData, const uint64_t timestamp);

    BoundingBox* convertBoundingBoxDataToBoundingBox(const ivbackend::BoundingBoxData& bboxData, int width, int height);

    DataFrame convertImageAndAnnotationToDataFrame(const ivbackend::ImageAndAnnotation& imageAndAnnotation);

    ivbackend::ImageAndAnnotation convertDataFrameToImageAndAnnotation(const DataFrame& dataFrame);

    void convertRegionOfInterestToRoi(const RegionOfInterest& regionOfInterest, ivbackend::Roi& roi);

    void convertBoundingBoxToBoundingBoxData(const BoundingBox& boundingBox, ivbackend::BoundingBoxData& bboxData);

    void convertImageToImageData(const Image& image, ivbackend::ImageData& imageData);

     /**
     * @brief Creates a ConfigContainer class.
     * @param configContainer The container to be converted into the class.
     * @return The ConfigContainer class.
     */
    static ConfigContainer createConfigContainerOutOfMessage(ivbackend::ConfigContainer configContainer);

    /**
     * @brief Creates a ConfigDetector class.
     * @param configDetector The dectector to be converted into the class.
     * @return The ConfigDetector class.
     */
    static ConfigDetector createConfigDetectorOutOfMessage(ivbackend::ConfigDetector configDetector);

    /**
     * @brief Creates a ConfigGStreamer class.
     * @param configDetector The gStreamer to be converted into the class.
     * @return The ConfigGStreamer class.
     */
    static ConfigGStreamer createConfigGStreamerOutOfMessage(ivbackend::ConfigGStreamer configGStreamer);


    /**
     * @brief Creates a Config class.
     * @param configMessage The configMessage to be converted into the class.
     * @return The Config class.
     */
    static Config createConfigOutOfMessage(ivbackend::ConfigResponseMessage configMessage);


    /**
     * @brief Creates a TimeRange class.
     * @param timeRange The timeRange to be converted into the class.
     * @return The TimeRange class.
     */
    static TimeRange createTimeRange(ivbackend::TimeRange timeRange);


    /**
     * @brief Creates Timeranges.
     * @param rangeInfo The rangeInfo to be converted into the class.
     * @return The TimeRanges first List at index 0 is onlyAnnotation.
     */
    static QVector<QVector<TimeRange*>>* createTimeRanges(ivbackend::RangeInfo rangeInfo);

    /**
     * @brief Creates List of Urls.
     * @param rangeInfo The rangeInfo to be converted into the class.
     * @return The List of Urls.
     */
    static QVector<QString>* createListOfStreamIdentifier(ivbackend::StreamIdentifierList streamList);


    /**
     * @brief Creates ivbackend TimeRange out of class.
     * @param time The TimeRange to be converted into the ivbackendtimerange.
     * @return The ivbackendtimerange.
     */
    static ivbackend::TimeRange createIvbackendTimeRange(TimeRange time);

     /**
     * @brief Creates ivbackend::InferenceType out of class.
     * @param type The InferenceType to be converted into the ivbackend::InferenceType.
     * @return The ivbackend::InferenceType.
     */
    static ivbackend::InferenceType getInferenceOutOfEnum(InferenceType type);

    /**
     * @brief Creates InferenceType out of ivbackend::InferenceType .
     * @param type The ivbackend::InferenceType to be converted into the InferenceType.
     * @return The InferenceType.
     */
    static InferenceType convertFromIvbackendInferenceType(ivbackend::InferenceType type);


    /**
     * @brief Creates ivbackend::ViewType out of ViewTypeEnum.
     * @param viewType The ViewTypeEnum to be converted into the ivbackend::ViewType.
     * @return The ivbackend::ViewType.
     */
    static ivbackend::ViewType convertToIvbackendViewType(ViewTypeEnum viewType);


    /**
     * @brief Creates ViewTypeEnum out of ivbackend::ViewType.
     * @param viewType The ivbackend::ViewType to be converted into the ViewTypeEnum.
     * @return The ViewTypeEnum.
     */
    ViewTypeEnum convertViewTypeFromIvbackend(ivbackend::ViewType viewType);

     /**
     * @brief Creates RoiList out of QVector<RegionOfInterest*>.
     * @param regionsOfInterest The QVector<RegionOfInterest*> to be converted into the RoiList.
     * @return The RoiList.
     */
    static ivbackend::RoiList convertRegionsToRoiList(QVector<RegionOfInterest*> regionsOfInterest);

};

#endif // MESSAGE_CONVERTER_H
