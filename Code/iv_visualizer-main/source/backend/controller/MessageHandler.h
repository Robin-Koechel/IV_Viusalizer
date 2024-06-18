#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include "ivbackend.grpc.pb.h"
#include "ivcontainer.grpc.pb.h"
#include "Config.h"
#include <grpcpp/grpcpp.h>
#include "ConfigDetector.h"
#include "ConfigContainer.h"
#include "ConfigGStreamer.h"
#include "MessageConverter.h"
#include "TimeRangeVector.h"
#include "DataManager.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <iterator>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <RegionOfInterest.h>

/**
 * @brief MessageHandler is the main interface for coordinating between frontend, backend and the processing tools. In this class, all requests from frontend get processes, data gets redirected to frontend as well as into the database
 */
class MessageHandler final : public ivbackend::IvBackendService::Service {
private:
    static MessageHandler* instance; /**< Singleton instance of MessageHandler. */

    /**
     * @brief Private constructor for the singleton pattern.
     */
    MessageHandler(); 

    // Store configurations using URL as key
    std::unordered_map<std::string, ivbackend::ConfigResponseMessage> configMap; /**< Map to store configurations using URL as key. */
    std::mutex configMutex; /**< Mutex for thread safety when modifying the available configs vector. */

    std::vector<std::string> availableUrls; /**< Vector to hold available URLs. */
    std::mutex urlMutex; /**< Mutex for thread safety when modifying the available URLs vector. */

    std::vector<std::pair<std::string, std::shared_ptr<grpc::Channel>>> channels; /**< Vector to hold gRPC channels identified by their URLs. */
    std::unordered_map<std::string, TimeRangeVector> availableVideos; /**< Map to store URLs mapped to TimeRanges(TimeRangeVector) for available images. */
    std::unordered_map<std::string, TimeRangeVector> availableAnnotations; /**< Map to store URLs mapped to TimeRanges(TimeRangeVector) for available annotations. */

    std::mutex roiMutex; /**< Mutex for thread safety when modifying the rois vector. */
    std::unordered_map<std::string, QVector<std::shared_ptr<iv_annotation::RegionOfInterest>>> roisMap; /**< Map to store URLs mapped to Rois. */

    

public:
    /**
     * @brief Gets the instance of the Singleton MessageHandler.
     * @return MessageHandler* The instance of MessageHandler.
     */
    static MessageHandler* getInstance();

    // Deleted copy constructor and assignment operator to prevent multiple instances
    MessageHandler(const MessageHandler&) = delete;
    MessageHandler& operator=(const MessageHandler&) = delete;

        /**
     * @brief Sets a configuration based on a gRPC request.
     *
     * This method retrieves images from the databank based on 
     * a start- and end timestamp. 
     * @param context The gRPC ServerContext.
     * @param request The gRPC ImageRequest.
     * @param writer The DataStream
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status getImages(grpc::ServerContext* context,
                           const ivbackend::ImageRequest* request,
                           grpc::ServerWriter<ivbackend::DataStream>* writer) override;

    /**
     * @brief Sets a configuration based on a gRPC request.
     *
     * This method starts an incoming stream into the database, for the case
     * where the user wants to have the data saved in the database, but not 
     * have them shown in the GUI
     * @param context The gRPC ServerContext.
     * @param request The gRPC request containing the StreamIdentifier.
     * @param response The gRPC InitialisationMessage reponse 
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status startIncomingStream(grpc::ServerContext* context,
                                     const ivbackend::StreamIdentifier* request,
                                     ivbackend::InitialisationMessage* response) override;

    /**
     * @brief Sets a configuration based on a gRPC request.
     *
     * This method redirects the retrieved images from the processing tools directly to the frontend, 
     * so they can be shown in the GUI.
     * @param context The gRPC ServerContext.
     * @param request The gRPC ImageRequest
     * @param writer gRPC stream 
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status getLiveImages(grpc::ServerContext* context,
                                               const ivbackend::ImageRequest* request,
                                               grpc::ServerWriter<ivbackend::DataStream>* writer) override;

    /**
     * @brief Sets a configuration based on a gRPC request.
     *
     * This method returns a list of all available processing tool URLs to the frontend
     * @param context The gRPC ServerContext.
     * @param request Empty gRPC request
     * @param response The gRPC StreamIdentifierList response.
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status getAvailableUrls(grpc::ServerContext* context, const ivbackend::Empty* request, ivbackend::StreamIdentifierList* response) override;

    /**
     * @brief Sets a configuration based on a gRPC request.
     *
     * This method extracts the Config object from the request, associates it with the
     * provided URL from the StreamIdentifier, and stores it in the configMap.
     *
     * @param context The gRPC ServerContext.
     * @param request The gRPC request containing the configuration data.
     * @param response The gRPC response (Empty) to be filled if needed.
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status setConfig(grpc::ServerContext* context,
                           const ivbackend::SetConfigMessage* request,
                           ivbackend::Empty* response) override;

    /**
     * @brief Gets a configuration based on a gRPC request.
     *
     * This method returns a Config for a certain URL
     *
     * @param context The gRPC ServerContext.
     * @param request The gRPC request containing a StreamIdentifier
     * @param response The gRPC ConfigResponseMessage.
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status getConfig(grpc::ServerContext* context,
                           const ivbackend::StreamIdentifier* request,
                           ivbackend::ConfigResponseMessage* response) override;

    /**
     * @brief Sets a configuration based on a gRPC request.
     *
     * Test method to send a dataFrame
     *
     * @param context The gRPC ServerContext.
     * @param request The empty gRPC.
     * @param response The gRPC DataStream response
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status sendTestDataFrame(grpc::ServerContext* context, const ivbackend::Empty* request, grpc::ServerWriter<ivbackend::DataStream>* writer) override;


    /**
     * @brief Converts ivcontainerpipeline::ImageData to ivbackend::DataStream.
     * 
     * This function takes an ImageData object from the ivcontainerpipeline package
     * and converts it into an DataStream object as defined in the ivbackend package.
     * The conversion involves copying the raw image data and the associated metadata
     * such as image dimensions.
     * 
     * @param srcImageData The ImageData object to be converted.
     * @return ivbackend::DataStream The converted DataStream object.
     */
    ivbackend::DataStream ConvertToDataStream(const ivcontainerpipeline::ImageData& srcImageData);

    /**
     * @brief Add a gRPC channel to the vector identified by its URL.
     * @param url The URL of the gRPC channel.
     * @param channel The gRPC channel to be added.
     */
    void addChannel(const std::string& url, const std::shared_ptr<grpc::Channel>& channel);

    /**
     * @brief Get a specific gRPC channel from the vector identified by its URL.
     * @param url The URL of the gRPC channel.
     * @return std::shared_ptr<grpc::Channel> The gRPC channel.
     */
    std::shared_ptr<grpc::Channel> getChannel(const std::string& url);

    /**
     * @brief Get or create a gRPC channel identified by its URL.
     * 
     * If a channel with the specified URL exists, it returns that channel.
     * Otherwise, it creates a new channel, adds it to the vector, and returns it.
     * 
     * @param url The URL of the gRPC channel.
     * @return std::shared_ptr<grpc::Channel> The gRPC channel.
     */
    std::shared_ptr<grpc::Channel> getOrCreateChannel(const std::string& url);

    /**
     * @brief Close a gRPC channel identified by its URL.
     * @param server_address The URL of the gRPC channel to close.
     */
    void closeChannel(const std::string& server_address);

    /**
     * @brief Test method to test server configuration
     *
     * This method extracts the Config object from the request, associates it with the
     * provided URL from the StreamIdentifier, and stores it in the configMap.
     *
     * @param context The gRPC ServerContext.
     * @param request The gRPC request.
     * @param response The gRPC response.
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status sayHello(grpc::ServerContext* context,
                                      const ivbackend::Empty* request,
                                      ivbackend::HelloResponse* response);

    /**
     * @brief Returns range of annotations and images available for a certain URL.
     *
     *
     * @param context The gRPC ServerContext.
     * @param request The gRPC request containing a StreamIdentifier
     * @param response The gRPC RangeInfo response.
     *
     * @return grpc::Status::OK upon successful configuration storage.
     */
    grpc::Status getRange(grpc::ServerContext* context,
                                      const ivbackend::StreamIdentifier* request,
                                      ivbackend::RangeInfo* response);

    /**
    * @brief Remove a URL from the list of available URLs.
    * @param url The URL to be removed.
    */
    void removeAvailableUrl(const std::string& url);

    /**
    * @brief Add a URL to the list of available URLs.
    * @param url The URL to be added.
    */
    void addAvailableUrl(const std::string& url);

    /**
     * @brief Internal getter for Configs based on URL.
     *
     * @param url URL to identify.
     *
     * @return ConfigResponseMessage.
     */
    std::optional<ivbackend::ConfigResponseMessage> getConfigForUrl(const std::string& url);

    /**
     * @brief Internal setter for configs based on URL.
     *  Maps ConfigResponseMessage to URL it belongs to
     * @param configResponse The gRPC configResponse.
     *
     * @return void
     */
    void setConfigForUrl(const std::string& url, const ivbackend::ConfigResponseMessage& configResponse);

    /**
    * @brief Stops the incoming stream associated with the provided server address.
    *
    * This method is responsible for stopping the data stream associated with the specified server address.
    * It utilizes the gRPC channel corresponding to the provided server address and invokes the Stop RPC
    * on the IvService to halt the data stream. Additional cleanup or logic may be performed based on
    * specific requirements.
    *
    * @param context The gRPC ServerContext for handling the RPC.
    * @param request The StreamIdentifier containing the server address to identify the stream to be stopped.
    * @param response The InitialisationMessage to be filled with any relevant response information.
    *
    * @return grpc::Status::OK if the stream is stopped successfully; otherwise, an appropriate error status.
    */
    grpc::Status stopIncomingStream(grpc::ServerContext* context,
                                                const ivbackend::StreamIdentifier* request,
                                                ivbackend::InitialisationMessage* response);

    std::string getChannelUrl(const std::shared_ptr<grpc::Channel>& channel);


    /**
     * @brief Internal getter for Rois based on URL.
     * @param url URL to identify.
     * @return the regions.
    */
    QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> getRoiForUrl(const std::string& url);


    /**
     * @brief Internal setter for rois based on URL.
     *  Maps Rois to URL it belongs to
     * @param url The url t set.
     * @param regionOfInterests the regions to set
     * @return void
     */
    void setRoiForUrl(const std::string& url, QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionOfInterests);

    void addTimeRangeToMap(std::unordered_map<std::string, TimeRangeVector>& timeRangeMap, const std::string& key, const TimeRange& timeRange);
    void replaceLastTimeRange(std::unordered_map<std::string, TimeRangeVector>& timeRangeMap, const std::string& key, const TimeRange& newTimeRange);
};

#endif // MESSAGEHANDLER_H
