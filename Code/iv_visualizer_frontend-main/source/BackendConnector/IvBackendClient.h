#ifndef IVBACKEND_CLIENT_H
#define IVBACKEND_CLIENT_H

#include <iostream>
#include "ivbackend.grpc.pb.h"
#include "MessageConverter.h"
#include "../FrameData/Config/ConfigContainer.h"
#include <chrono>
#include <thread>

/**
 * @brief The IvBackendClient class represents a client for interacting with the IvBackend service.
 */
class IvBackendClient {
public:
    /**
     * @brief Constructs an IvBackendClient object.
     */
    IvBackendClient();

    /**
     * @brief Sets the server address for the IvBackend service.
     * @param serverAddress The address of the IvBackend service.
     */
    void SetServerAddress(const std::string& serverAddress);

    /**
     * @brief Starts the incoming stream from the IvBackend service.
     * @return false if stream couldnt be started
     */
    bool StartIncomingStream(const ivbackend::StreamIdentifier& streamIdentifier);

    /**
     * @brief Sets the configuration for the IvBackend service.
     * @param configResponse The configuration response message.
     * @param streamIdentifier the identifier for the stream that gets changed
     */
    void SetConfig(const ivbackend::ConfigResponseMessage& configResponse, const ivbackend::StreamIdentifier& streamIdentifier);

    /**
     * @brief Gets the configuration from the IvBackend service.
     * @return The configuration response message.
     */
    ivbackend::ConfigResponseMessage GetConfig(const ivbackend::StreamIdentifier& streamIdentifier);

    ivbackend::DataStream GetTestDataFrame();

    /**
     * @brief Gets images from the IvBackend service.
     * @param callback A callback function to handle the received image stream.
     */
    bool GetImages(const ivbackend::ImageRequest& request, std::function<void(const ivbackend::DataStream&)> callback);

    /**
     * @brief Gets information about the time range for videos and annotations.
     * @return The range information.
     */
    ivbackend::RangeInfo GetRange(const ivbackend::StreamIdentifier& streamIdentifier);

    /**
     * @brief Gets the list of available URLs from the IvBackend service.
     * @return The list of available stream identifiers.
     */
    ivbackend::StreamIdentifierList GetAvailableUrls();

    /**
     * @brief Gets live images from the IvBackend service.
     * @param request The image request specifying the stream and time range.
     * @param callback A callback function to handle the received live image stream.
     */
    bool GetLiveImages(const ivbackend::ImageRequest& request, std::function<void(const ivbackend::DataStream&)> callback);

    void StopLiveStream(const ivbackend::StreamIdentifier& streamIdentifier);

    void StopIncomingStream(const ivbackend::StreamIdentifier& streamIdentifier);

    void StopLiveStream();

private:
    std::unique_ptr<ivbackend::IvBackendService::Stub> stub_; /**< gRPC stub for communication with the IvBackend service. */
    std::string serverAddress_; /**< Address of the IvBackend service. */
    std::atomic<bool> stopStreaming{false};
};

#endif // IVBACKEND_CLIENT_H