#include "MessageHandler.h"
#include <algorithm> 

MessageHandler* MessageHandler::instance = nullptr;

MessageHandler::MessageHandler() {

  channels.clear();
  configMap.clear();
}

MessageHandler* MessageHandler::getInstance() {
    static std::unique_ptr<MessageHandler> instance(new MessageHandler());
    return instance.get();
}

grpc::Status MessageHandler::getImages(grpc::ServerContext* context,
                                       const ivbackend::ImageRequest* request,
                                       grpc::ServerWriter<ivbackend::DataStream>* writer) {
  std::string server_address = request->identifier().url();
  auto dataManager = DataManager::getInstance(QString::fromStdString(server_address));
  std::shared_ptr<grpc::Channel> channel = getOrCreateChannel(server_address);
  
  // Perform the requested call on the channel
  std::unique_ptr<ivcontainerpipeline::IvService::Stub> stub = ivcontainerpipeline::IvService::NewStub(channel);
    
  uint64_t startTime = request->range().start();
  uint64_t endTime = request->range().end();
  auto outputQueue = dataManager->getOutputQueue(startTime, endTime);

  while (true) {
    std::shared_ptr<DataFrame> dataFrame;
    if (outputQueue->Consume(dataFrame)) {
        // Process dataFrame
        ivbackend::DataStream dataStream = MessageConverter::ConvertDataFrameToDataStream(*dataFrame);
        writer->Write(dataStream);
    } else if(outputQueue->IsEmpty()) {
        return grpc::Status::OK;
        // TODO handle this better
    }
    }
  return grpc::Status::OK;
}

grpc::Status MessageHandler::getLiveImages(grpc::ServerContext* context,
                                           const ivbackend::ImageRequest* request,
                                           grpc::ServerWriter<ivbackend::DataStream>* writer) {

    std::string server_address = request->identifier().url();
    std::shared_ptr<grpc::Channel> channel = getOrCreateChannel(server_address);
    std::unique_ptr<ivcontainerpipeline::IvService::Stub> stub = ivcontainerpipeline::IvService::NewStub(channel);

    // Create an asynchronous client reader-writer for streaming
    grpc::ClientContext clientContext;
    grpc::CompletionQueue cq;
    grpc::Status status;

    // Initiate the RPC and create a handle for it. Bind the RPC to a CompletionQueue.
    auto streamAsync = stub->AsyncStart(&clientContext, ivcontainerpipeline::Empty{}, &cq, (void*)1);

    // Ask for the reply and final status, with a unique tag
    ivcontainerpipeline::DataStream dataStream;
    streamAsync->Read(&dataStream, (void*)1);

    // Wait for the completion queue to return the next tag. The reply and status are ready once the tag passed into the corresponding Read() call is returned.
    void* got_tag;
    bool ok = false;

    // Wait for the completion queue to return the next tag.
    cq.Next(&got_tag, &ok);
    if (ok && got_tag == (void*)1) {
        // Handle the initial acknowledgment or error
        if (status.ok()) {
            // Process the initial acknowledgment and start reading data
            while (true) {
                // Process dataStream and write to the original caller
                // TODO convert ivcontainerpipeline to ivbackend datastream check 
                ivbackend::DataStream dS = MessageConverter::ConvertToDataStream(dataStream);
                writer->Write(dS);
                std::cout << "Data sent" << std::endl;

                // Wait for the completion queue to return the next tag
                cq.Next(&got_tag, &ok);
                if (!ok) {
                    // Handle error during the read operation
                    std::cerr << "Error during data stream read operation." << std::endl;
                    break;
                }

                if (got_tag == (void*)1) {
                    // Continue reading for more data
                    streamAsync->Read(&dataStream, (void*)1);
                } else {
                    // Break the loop on other tags
                    break;
                }
            }
        } else {
            // Handle error during the AsyncStart operation
            std::cerr << "Error during AsyncStart operation. Code: " << status.error_code() << ", Message: " << status.error_message() << std::endl;
        }
    } else {
        // Handle unexpected tag in completion queue
        std::cerr << "Unexpected tag in completion queue: " << got_tag << std::endl;
    }

    // Finish the stream
    grpc::Status finishStatus;
    streamAsync->Finish(&finishStatus, (void*)3);

    if (!finishStatus.ok()) {
        // TODO Handle error if stream ends with an error
        std::cerr << "Stream ended with error: " << finishStatus.error_message() << std::endl;
        return finishStatus;
    }

    // Block until Finish completes
    cq.Next(&got_tag, &ok);

    return grpc::Status::OK;
}


grpc::Status MessageHandler::startIncomingStream(grpc::ServerContext* context,
                                                 const ivbackend::StreamIdentifier* request,
                                                 ivbackend::InitialisationMessage* response) {
    // Extract the server address from the request
    std::string server_address = request->url();
    {
        std::lock_guard<std::mutex> lock(urlMutex);
        auto it = std::find(availableUrls.begin(), availableUrls.end(), server_address);
        if(it != availableUrls.end()){
            return grpc::Status::OK;
        }
    } 
    // TODO: Error handling in case URL is not available
    // Health check in container proto? 
    addAvailableUrl(server_address);

    auto dataManager = DataManager::getInstance(QString::fromStdString(server_address));
    addTimeRangeToMap(availableVideos, server_address, dataManager->getTimeRangeVideos());
    addTimeRangeToMap(availableAnnotations, server_address, dataManager->getTimeRangeAnnotations());
    
    
    // Get or create a gRPC channel for communication
    std::shared_ptr<grpc::Channel> channel = getOrCreateChannel(server_address);

    // Create a stub for the IvService
    std::unique_ptr<ivcontainerpipeline::IvService::Stub> stub = ivcontainerpipeline::IvService::NewStub(channel);
    
    // Call the Init RPC to initialize the services and get the current configuration
    ivcontainerpipeline::Empty initRequest;
    ivcontainerpipeline::ConfigResponseMessage configResponse;
    grpc::ClientContext initContext;
    grpc::Status initStatus = stub->Init(&initContext, initRequest, &configResponse);

     // Convert ivcontainerpipeline::ConfigResponseMessage to ivbackend::ConfigResponseMessage
    ivbackend::ConfigResponseMessage backendConfigResponse = MessageConverter::ConvertToBackendConfigResponse(configResponse);

    // Save the received configuration for future use
    configMap[server_address] = backendConfigResponse;
    roisMap[server_address] = MessageConverter::extractRegionsOfInterestIvBackend(backendConfigResponse);
    if (!initStatus.ok()) {
        // TODO Handle error if initialization fails
        std::cerr << "Initialization failed: " << initStatus.error_message() << std::endl;
        return initStatus;
    }

    int fps = getConfigForUrl(server_address)->config_gstreamer().target_fps();

    // Call the Start RPC to begin the data stream
    ivcontainerpipeline::Empty startRequest;
    grpc::ClientContext startContext;
    std::unique_ptr<grpc::ClientReader<ivcontainerpipeline::DataStream>> stream = stub->Start(&startContext, startRequest);

    // Process incoming messages from the data stream
    ivcontainerpipeline::DataStream dataStream;
    bool init = true;
    uint64_t startTimestamp = 0;
    int x = 0;
    while (stream->Read(&dataStream)) {
      std::shared_ptr<DataFrame> dataFrame = MessageConverter::ConvertDataStreamToDataFrame(dataStream, fps, QString::fromStdString(server_address));

        if(init) {
            startTimestamp = dataFrame->getImage()->get_timestamp();
            TimeRange t(startTimestamp, startTimestamp, fps);
            addTimeRangeToMap(availableVideos, server_address, t);
            addTimeRangeToMap(availableAnnotations, server_address, t);
            init = false;
        } else {
            uint64_t endTimeStamp = dataFrame->getImage()->get_timestamp();
            TimeRange t(startTimestamp, endTimeStamp, fps);
            replaceLastTimeRange(availableVideos, server_address, t);
            replaceLastTimeRange(availableAnnotations, server_address, t);
        }
        std::cout << "Received data from stream. " << x++ << "  timestamp: " << dataFrame->getImage()->get_timestamp() << std::endl;
        dataFrame->setRegionsOfInterest(getRoiForUrl(server_address));
        dataManager->addDataFrame(dataFrame);

    }

    // Check for any errors in the stream
    grpc::Status status = stream->Finish();
    if (!status.ok()) {
        // TODO Handle error if stream ends with an error
        std::cerr << "Stream ended with error: " << status.error_message() << std::endl;
        return status;
    }
    // Return OK status
    return grpc::Status::OK;
}

grpc::Status MessageHandler::sendTestDataFrame(grpc::ServerContext* context, const ivbackend::Empty* request, grpc::ServerWriter<ivbackend::DataStream>* writer) {
    // Simulate an image stream by sending a data frame every second with a white image and a few bounding boxes
    while (true) {
        ivbackend::ImageData imageData;
        imageData.mutable_meta_data()->set_width(640);
        imageData.mutable_meta_data()->set_height(480);

        ivbackend::BoundingBoxList boundingBoxList;
        for (int i = 0; i < 3; ++i) {
            ivbackend::BoundingBoxData* boundingBox = boundingBoxList.add_items();
            boundingBox->set_x1(50 + i * 50);
            boundingBox->set_y1(50);
            boundingBox->set_x2(100 + i * 50);
            boundingBox->set_y2(100);
            boundingBox->set_confidence(0.8);
            boundingBox->set_label("Object " + std::to_string(i + 1));
        }

        ivbackend::ImageAndAnnotation imageAndAnnotation;
        imageAndAnnotation.mutable_boundingboxes()->CopyFrom(boundingBoxList);
        imageAndAnnotation.mutable_image()->CopyFrom(imageData);
        imageAndAnnotation.set_timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

        ivbackend::DataStream dataFrame;
        dataFrame.mutable_data()->CopyFrom(imageAndAnnotation);

        // Send the data frame to the client 
        writer->Write(dataFrame);

        // Wait for one second before sending the next data frame
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // This line will never be reached, as the loop is infinite
    return grpc::Status::OK;
}

grpc::Status MessageHandler::setConfig(grpc::ServerContext* context,
                                       const ivbackend::SetConfigMessage* request,
                                       ivbackend::Empty* response) {

  // Extract the URL from the StreamIdentifier
  std::string server_address = request->identifier().url();

  // Close the existing channel if it exists
  closeChannel(server_address);

  // Create a new ConfigResponseMessage
  ivbackend::ConfigResponseMessage receivedConfig;

  // Populate receivedConfig
  MessageConverter::PopulateConfigResponse(&receivedConfig, &request->config());

  configMap[server_address] = receivedConfig;

  // Basically same class, just from different protos
  // Convert ivbackend::ConfigResponseMessage to ivcontainerpipeline::SetConfig
  ivcontainerpipeline::SetConfig containerPipelineConfig = MessageConverter::ConvertToIvContainerPipelineSetConfig(request->config());

  // Create a gRPC channel and stub for processing tools service
  std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
  std::unique_ptr<ivcontainerpipeline::IvService::Stub> stub = ivcontainerpipeline::IvService::NewStub(channel);

  // Add the new channel to the channels vector
  channels.emplace_back(server_address, channel);

  // Create a gRPC context and response object
  grpc::ClientContext rpcContext;
  ivcontainerpipeline::ConfigResponseMessage configResponse;

  // Make the RPC call (UpdateConfig) with the converted config
  grpc::Status rpcStatus = stub->UpdateConfig(&rpcContext, containerPipelineConfig, &configResponse);
  setRoiForUrl(server_address, MessageConverter::extractRegionsOfInterestIvBackend(request->config()));
  return grpc::Status::OK;
}

grpc::Status MessageHandler::getConfig(grpc::ServerContext* context,
                                       const ivbackend::StreamIdentifier* request,
                                       ivbackend::ConfigResponseMessage* response) {
  // Extract URL from request
  std::string url = request->url();
  
  // Search for Config object in configMap
  auto it = configMap.find(url);

  if (it != configMap.end()) {
    // If found, copy the stored ConfigResponseMessage to the response
    response->CopyFrom(it->second);

    // Return a success status
    return grpc::Status::OK;
  } else {
    // If not found, return an error status
    return grpc::Status(grpc::StatusCode::NOT_FOUND, "Config not found for the specified URL");
  }
}

std::shared_ptr<grpc::Channel> MessageHandler::getChannel(const std::string& url) {
    for (const auto& pair : channels) {
        if (pair.first == url) {
            return pair.second;
        }
    }
    return nullptr;
}

void MessageHandler::addChannel(const std::string& url, const std::shared_ptr<grpc::Channel>& channel) {
    channels.emplace_back(std::make_pair(url, channel));
}

std::shared_ptr<grpc::Channel> MessageHandler::getOrCreateChannel(const std::string& url) {
    // Check if the channel already exists
    std::shared_ptr<grpc::Channel> channel = getChannel(url);
    if (!channel) {
        grpc::ChannelArguments channelArgs;
        
        channelArgs.SetMaxReceiveMessageSize(-1);

        channel = grpc::CreateCustomChannel(url, grpc::InsecureChannelCredentials(), channelArgs);

        addChannel(url, channel);
    }
    return channel;
}

void MessageHandler::closeChannel(const std::string& server_address) {
    auto it = std::find_if(channels.begin(), channels.end(), [server_address](const auto& pair) {
        return pair.first == server_address;
    });

    if (it != channels.end()) {
        // Close the channel if found
        it->second.reset();
        channels.erase(it);
    }
}

grpc::Status MessageHandler::getAvailableUrls(grpc::ServerContext* context, const ivbackend::Empty* request, ivbackend::StreamIdentifierList* response) {
    // Use a lock to ensure thread safety when accessing the vector
    std::lock_guard<std::mutex> lock(urlMutex);

    // Copy the available URLs to the response
    for (const auto& url : availableUrls) {
        auto* streamIdentifier = response->add_urls();
        streamIdentifier->set_url(url);
    }

    return grpc::Status::OK;
}


grpc::Status MessageHandler::sayHello(grpc::ServerContext* context,
                                      const ivbackend::Empty* request,
                                      ivbackend::HelloResponse* response) {
    response->set_message("Hello, world!");
    return grpc::Status::OK;
}

grpc::Status MessageHandler::getRange(grpc::ServerContext* context,
                                      const ivbackend::StreamIdentifier* request,
                                      ivbackend::RangeInfo* response) {
    std::string url = request->url();

    // Fetch TimeRangeVector from availableVideos map
    auto videosIt = availableVideos.find(url);
    if (videosIt != availableVideos.end()) {
        TimeRangeVector videoTimeRangeVector = videosIt->second;

    // Convert each TimeRange to ivbackend::TimeRange
    std::vector<ivbackend::TimeRange> videoTimeRanges;
    for (const auto& timeRanges : videoTimeRangeVector.getTimeRanges()) {
        for (const auto& timeRange : timeRanges) {
            if(timeRange.getStartTime() != 0){
                videoTimeRanges.push_back(MessageConverter::convertToProtoTimeRange(timeRange));
            }        }
    }

    // Set the response with the obtained video time ranges
    auto mutableVideoRange = response->mutable_range_videos();
    auto mutableVideos = mutableVideoRange->mutable_videos();
    mutableVideos->Reserve(videoTimeRanges.size());
    for (const auto& timeRange : videoTimeRanges) {
        mutableVideos->Add()->CopyFrom(timeRange);   
        }
    }

    // Fetch TimeRangeVector from availableAnnotations map
    auto annotationsIt = availableAnnotations.find(url);
    if (annotationsIt != availableAnnotations.end()) {
        TimeRangeVector annotationTimeRangeVector = annotationsIt->second;

    // Convert each TimeRange to ivbackend::TimeRange
    std::vector<ivbackend::TimeRange> annotationTimeRanges;
    for (const auto& timeRanges : annotationTimeRangeVector.getTimeRanges()) {
        for (const auto& timeRange : timeRanges) {
            if(timeRange.getStartTime() != 0){
                annotationTimeRanges.push_back(MessageConverter::convertToProtoTimeRange(timeRange));
            }
        }
    }

    // Set the response with the obtained annotation time ranges
    auto mutableAnnotationRange = response->mutable_range_annotations();
    auto mutableAnnotations = mutableAnnotationRange->mutable_annotations();
    mutableAnnotations->Reserve(annotationTimeRanges.size());
    for (const auto& timeRange : annotationTimeRanges) {
        mutableAnnotations->Add()->CopyFrom(timeRange);
    }
}

    return grpc::Status::OK;
}

void MessageHandler::addAvailableUrl(const std::string& url) {
    std::lock_guard<std::mutex> lock(urlMutex);
    availableUrls.push_back(url);
}

void MessageHandler::removeAvailableUrl(const std::string& url) {
    std::lock_guard<std::mutex> lock(urlMutex);
    // Find and erase the URL from the vector
    auto it = std::find(availableUrls.begin(), availableUrls.end(), url);
    if (it != availableUrls.end()) {
        availableUrls.erase(it);
    }
}

std::optional<ivbackend::ConfigResponseMessage> MessageHandler::getConfigForUrl(const std::string& url) {
        std::lock_guard<std::mutex> lock(configMutex);
        auto it = configMap.find(url);
        if (it != configMap.end()) {
            return it->second;
        }
        return std::nullopt;
}

void MessageHandler::setConfigForUrl(const std::string& url, const ivbackend::ConfigResponseMessage& configResponse) {
    std::lock_guard<std::mutex> lock(configMutex);
    configMap[url] = configResponse;
}

QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> MessageHandler::getRoiForUrl(const std::string& url) {
        std::lock_guard<std::mutex> lock(roiMutex);
        auto it = roisMap.find(url);
        if (it != roisMap.end()) {
            return it->second;
        }
        return QVector<std::shared_ptr<iv_annotation::RegionOfInterest>>();
}

void MessageHandler::setRoiForUrl(const std::string& url, QVector<std::shared_ptr<iv_annotation::RegionOfInterest>> regionOfInterests) {
    std::lock_guard<std::mutex> lock(roiMutex);
    roisMap[url] = regionOfInterests;
}

grpc::Status MessageHandler::stopIncomingStream(grpc::ServerContext* context,
                                                const ivbackend::StreamIdentifier* request,
                                                ivbackend::InitialisationMessage* response) {
    // Extract the server address from the request
    std::string server_address = request->url();

    // Retrieve the gRPC channel for the specified server_address
    std::shared_ptr<grpc::Channel> channel = getChannel(server_address);

    // Check if the channel exists
    if (channel) {
        // Create a stub for the IvService
        std::unique_ptr<ivcontainerpipeline::IvService::Stub> stub = ivcontainerpipeline::IvService::NewStub(channel);

        // Call the Stop RPC to stop the data stream
        ivcontainerpipeline::Empty stopRequest;
        grpc::ClientContext stopContext;
        ivcontainerpipeline::InfoMessage stopResponse;
        grpc::Status stopStatus = stub->Stop(&stopContext, stopRequest, &stopResponse);

        if (!stopStatus.ok()) {
            // Handle error if stopping the stream fails
            std::cerr << "Stopping stream failed: " << stopStatus.error_message() << std::endl;
            return stopStatus;
        }

        // Remove the server address from the available URLs
        removeAvailableUrl(server_address);

        // Return OK status
        return grpc::Status::OK;
    } else {
        // Handle the case where the channel does not exist (not started or already stopped)
        std::cerr << "Channel not found for server address: " << server_address << std::endl;
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Channel not found for the specified server address");
    }
}

std::string MessageHandler::getChannelUrl(const std::shared_ptr<grpc::Channel>& channel) {
    for (const auto& pair : channels) {
        if (pair.second == channel) {
            return pair.first;
        }
    }
    return "";  // Return an empty string or handle the case where the channel is not found
}

void MessageHandler::addTimeRangeToMap(std::unordered_map<std::string, TimeRangeVector>& timeRangeMap, const std::string& key, const TimeRange& timeRange) {
    auto it = timeRangeMap.find(key);

    if (it != timeRangeMap.end()) {
        // If the key exists, add the TimeRange to the existing TimeRangeVector
        it->second.addTimeRange(timeRange);
    } else {
        // If the key doesn't exist, create a new entry with the TimeRangeVector containing the TimeRange
        TimeRangeVector timeRangeVector;
        timeRangeVector.addTimeRange(timeRange);
        timeRangeMap.emplace(key, timeRangeVector);
    }
}

void MessageHandler::replaceLastTimeRange(std::unordered_map<std::string, TimeRangeVector>& timeRangeMap, const std::string& key, const TimeRange& newTimeRange) {
    auto it = timeRangeMap.find(key);

    if (it != timeRangeMap.end() && !it->second.getTimeRanges().empty()) {
        // If the key exists and the TimeRangeVector is not empty, replace the last TimeRange
        auto& timeRanges = it->second.getTimeRanges();
        timeRanges.back().pop_back();
        timeRanges.back().emplace_back(newTimeRange);
    } else {
        std::cerr << "Error: Cannot replace last TimeRange. Key not found or TimeRangeVector is empty." << std::endl;
    }
}