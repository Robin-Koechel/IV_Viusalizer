#ifndef MESSAGE_CONVERTER_H
#define MESSAGE_CONVERTER_H

#include <grpcpp/grpcpp.h>
#include "ivbackend.grpc.pb.h"
#include "ivcontainer.grpc.pb.h"
#include <RegionOfInterest.h>
#include "../common/Image.h"
#include <BoundingBox.h>
#include "../common/DataFrame.h"
#include "../common/TimeRange.h"
#include <QString>

/**
 * @brief A utility class for converting basic data types to and from gRPC message types to support the MessageConverter.
 */
class MessageConverter {
public:


        /**
     * @brief Converts an ivbackend::ConfigResponseMessage to an ivcontainerpipeline::SetConfig.
     *
     * @param sourceConfig The source configuration message.
     * @return ivcontainerpipeline::SetConfig The converted SetConfig message.
     */
    static ivcontainerpipeline::SetConfig ConvertToIvContainerPipelineSetConfig(const ivbackend::ConfigResponseMessage& sourceConfig);

    /**
     * @brief Populates an ivbackend::ConfigResponseMessage based on another instance.
     *
     * @param receivedConfig The destination ConfigResponseMessage.
     * @param request The source ConfigResponseMessage.
     */
    static void PopulateConfigResponse(ivbackend::ConfigResponseMessage* receivedConfig, const ivbackend::ConfigResponseMessage* request);

    /**
     * @brief Converts an ivcontainerpipeline::DataStream to an ivbackend::DataStream.
     *
     * @param srcImageData The source DataStream from ivcontainerpipeline.
     * @return ivbackend::DataStream The converted DataStream for ivbackend.
     */
    static ivbackend::DataStream ConvertToDataStream(const ivcontainerpipeline::DataStream& srcImageData);

    /**
     * @brief Converts ivcontainerpipeline::DataStream to DataFrame for ivbackend.
     *
     * @param dataStream The source DataStream from ivcontainerpipeline.
     * @param fps The frames per second.
     * @param url The URL associated with the data.
     * @return DataFrame The converted DataFrame.
     */
    static std::shared_ptr<DataFrame> ConvertDataStreamToDataFrame(const ivcontainerpipeline::DataStream& dataStream, const int& fps, const QString& url);

    // /**
    //  * @brief Converts ivcontainerpipeline::ConfigResponseMessage to ivbackend::ConfigResponseMessage.
    //  *
    //  * @param sourceConfig The source configuration message.
    //  * @return ivbackend::ConfigResponseMessage The converted ConfigResponseMessage.
    //  */
    // static ivbackend::ConfigResponseMessage ConvertToBackendConfigResponse(const ivcontainerpipeline::ConfigResponseMessage& sourceConfig);

    
    /**
     * @brief Converts ivcontainerpipeline::ImageData to common Image type.
     *
     * @param imageData The source ImageData from ivcontainerpipeline.
     * @param timestamp The timestamp associated with the image.
     * @return Image The converted Image.
     */
    static std::shared_ptr<Image> convertDataStreamToImage(const ivcontainerpipeline::ImageData& imageData, uint64_t timestamp);

    /**
     * @brief Converts ivcontainerpipeline::BoundingBoxData to iv_annotation::BoundingBox.
     *
     * @param bounding_box The source BoundingBoxData from ivcontainerpipeline.
     * @return iv_annotation::BoundingBox* The converted BoundingBox.
     */
    static std::shared_ptr<iv_annotation::BoundingBox> ConvertContainerBoundingBoxToAnnotationBoundingBox(const ivcontainerpipeline::BoundingBoxData& bounding_box);

    /**
     * @brief Converts ivcontainerpipeline::Roi to iv_annotation::RegionOfInterest.
     *
     * @param roi The source RegionOfInterest from ivcontainerpipeline.
     * @return iv_annotation::RegionOfInterest* The converted RegionOfInterest.
     */
    static std::shared_ptr<iv_annotation::RegionOfInterest> ConvertContainerRegionOfInterestToAnnotationRegionOfInterest(const ivcontainerpipeline::Roi& roi);

    /**
     * @brief Converts common TimeRange to ivbackend::TimeRange.
     *
     * @param timeRange The source TimeRange.
     * @return ivbackend::TimeRange The converted TimeRange for ivbackend.
     */
    static ivbackend::TimeRange convertToProtoTimeRange(const TimeRange& timeRange);

     /**
     * @brief Converts ConfigContainer out of backend Proto to ConfigContainer out of ivcontainerpipeline Proto
     * @param container the configContainer out of ivbackend
     * @return ConfigContainer out of ivcontainerpipeline
    */
    static ivcontainerpipeline::ConfigContainer ConvertContainerFromBackendToPipeline(const ivbackend::ConfigContainer& container);

    /**
     * @brief Converts ConfigDetector out of backend Proto to ConfigDetector out of ivcontainerpipeline Proto
     * @param detector the ConfigDetector out of ivbackend
     * @return ConfigDetector out of ivcontainerpipeline
    */
    static ivcontainerpipeline::ConfigDetector  ConvertDetectorFromBackendToPipeline(const ivbackend::ConfigDetector & detector);


    /**
     * @brief Converts ConfigGStreamer out of backend Proto to ConfigGStreamer out of ivcontainerpipeline Proto
     * @param gStreamer the ConfigGStreamer out of ivbackend
     * @return ConfigGStreamer out of ivcontainerpipeline
    */
    static ivcontainerpipeline::ConfigGStreamer ConvertGStreamerFromBackendToPipeline(const ivbackend::ConfigGStreamer & gStreamer);

    /**
     * @brief Converts InferenceType out of backend Proto to InferenceType out of ivcontainerpipeline Proto
     * @param type the InferenceType out of ivbackend
     * @return InferenceType out of ivcontainerpipeline
    */
    static ivcontainerpipeline::InferenceType ConvertInferenceTypeFromBackendToPipeline(const ivbackend::InferenceType& type);

    /**
     * @brief Converts RoiType out of backend Proto to RoiType out of ivcontainerpipeline Proto
     * @param type the RoiType out of ivbackend
     * @return RoiType out of ivcontainerpipeline
    */
    static ivcontainerpipeline::RoiType ConvertRoiTypeFromBackendToPipeline(const ivbackend::RoiType & type);

    static ivcontainerpipeline::ConfigResponseMessage ConvertToContainerConfigResponse(const ivbackend::ConfigResponseMessage& backendConfig);

    static ivbackend::ConfigResponseMessage ConvertToBackendConfigResponse(const ivcontainerpipeline::ConfigResponseMessage& containerConfig);

    static ivbackend::DataStream ConvertDataFrameToDataStream(DataFrame& dataFrame);

    static std::shared_ptr<ivbackend::ImageData> convertImageToImageData(const std::shared_ptr<Image> image);

    static ivbackend::BoundingBoxData ConvertToBackendBoundingBox(const ivcontainerpipeline::BoundingBoxData& srcBoundingBox);

    static ivbackend::BoundingBoxList ConvertToBackendBoundingBoxList(const ivcontainerpipeline::BoundingBoxList& srcBoundingBoxList);

    static ivbackend::RoiList ConvertToBackendRoiList(const ivcontainerpipeline::RoiList& srcRoiList);

    static ivbackend::Roi ConvertToBackendRoi(const ivcontainerpipeline::Roi& srcRoi);

    static ivbackend::Point ConvertToBackendPoint(const ivcontainerpipeline::Point& srcPoint);

    static QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> extractRegionsOfInterestIvBackend(const ivbackend::ConfigResponseMessage& configResponse);

    static QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> extractRegionsOfInterestIvContainer(const ivcontainerpipeline::ConfigResponseMessage& configResponse);
};

#endif // MESSAGE_CONVERTER_H
