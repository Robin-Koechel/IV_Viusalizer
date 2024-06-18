#include "IvBackendClient.h"
#include <grpcpp/grpcpp.h>

IvBackendClient::IvBackendClient() : stub_(nullptr) {}

// TODO Possibly introduce more state? 
// Would be useful to know if IncomingStream is running for example

void IvBackendClient::SetServerAddress(const std::string& serverAddress) {
   
        serverAddress_ = serverAddress;
        
        grpc::ChannelArguments channelArgs;
        
        channelArgs.SetMaxReceiveMessageSize(-1); 
        
        auto channel = grpc::CreateCustomChannel(serverAddress_, grpc::InsecureChannelCredentials(), channelArgs);

        stub_ = ivbackend::IvBackendService::NewStub(channel);
}

bool IvBackendClient::StartIncomingStream(const ivbackend::StreamIdentifier& streamIdentifier) {
        if (serverAddress_.empty()) {
            // TODO 
            std::cerr << "Server address not set. Call SetServerAddress first." << std::endl;
        }

        grpc::ClientContext context;
        ivbackend::InitialisationMessage initMessage;

        grpc::Status status = stub_->startIncomingStream(&context, streamIdentifier, &initMessage);

        if (status.ok()) {
            stopStreaming = false;
            std::cout << "Start Incoming Stream successful." << std::endl;
            return true;
        } else {
            std::cerr << "Error starting incoming stream: " << status.error_message() << std::endl;
            return false;
        }
}

void IvBackendClient::StopIncomingStream(const ivbackend::StreamIdentifier& streamIdentifier) {

    if (serverAddress_.empty()) {
        std::cerr << "Server address not set. Call SetServerAddress first." << std::endl;
        // TODO 
    }

    grpc::ClientContext context;
    ivbackend::InitialisationMessage initMessage;

    grpc::Status status = stub_->stopIncomingStream(&context, streamIdentifier, &initMessage);

    if (status.ok()) {
        stopStreaming = true;
        std::cout << "Stopped incoming Stream successful." << std::endl;
    } else {
        std::cerr << "Error stopping incoming stream: " << status.error_message() << std::endl;
    }
}

// StopIncomingStream

void IvBackendClient::SetConfig(const ivbackend::ConfigResponseMessage& configResponse, const ivbackend::StreamIdentifier& streamIdentifier) {
    grpc::ClientContext context;
    ivbackend::Empty emptyResponse;
    ivbackend::SetConfigMessage message;
    ivbackend::ConfigResponseMessage* configPtr = message.mutable_config();
    configPtr->Swap(const_cast<ivbackend::ConfigResponseMessage*>(&configResponse));
    ivbackend::StreamIdentifier* streamIdPtr = message.mutable_identifier();
    streamIdPtr->Swap(const_cast<ivbackend::StreamIdentifier*>(&streamIdentifier));
    for (const auto& roi : message.config().config_detector().rois().items()) {
       int b = 0;
    }

    grpc::Status status = stub_->setConfig(&context, message, &emptyResponse);
    if (status.ok()) {
        std::cout << "Set Config successful." << std::endl;
    } else {
        std::cerr << "Error setting configuration: " << status.error_message() << std::endl;
    }
}

ivbackend::ConfigResponseMessage IvBackendClient::GetConfig(const ivbackend::StreamIdentifier& streamIdentifier) {

    if (serverAddress_.empty()) {
        std::cerr << "Server address not set. Call SetServerAddress first." << std::endl;
        // TODO
        // Maybe return currentConfig? 
        return ivbackend::ConfigResponseMessage();
    }

    grpc::ClientContext context;
    ivbackend::ConfigResponseMessage configResponse;

    grpc::Status status = stub_->getConfig(&context, streamIdentifier, &configResponse);

    if (status.ok()) {
        std::cout << "Get Config successful." << std::endl;
    } else {
        std::cerr << "Error getting configuration: " << status.error_message() << std::endl;
    }

    return configResponse;
}

// Add timestamp
bool IvBackendClient::GetImages(const ivbackend::ImageRequest& request, std::function<void(const ivbackend::DataStream&)> callback) {
    grpc::ClientContext context;
    ivbackend::DataStream dataStream;

    std::unique_ptr<grpc::ClientReader<ivbackend::DataStream>> reader = stub_->getImages(&context, request);

    while (!stopStreaming && reader->Read(&dataStream)) {
        callback(dataStream);
    }
    stopStreaming = true;

    grpc::Status status = reader->Finish();

    if (status.ok()) {
        std::cout << "Get Images successful." << std::endl;
        return true;

    } else {
        std::cerr << "Error getting images: " << status.error_message() << std::endl;
        return false;
    }
}

ivbackend::RangeInfo IvBackendClient::GetRange(const ivbackend::StreamIdentifier& streamIdentifier) {

    if (serverAddress_.empty()) {
        std::cerr << "Server address not set. Call SetServerAddress first." << std::endl;
        return ivbackend::RangeInfo();
        // TODO
    }

    grpc::ClientContext context;
    ivbackend::RangeInfo rangeInfo;

    grpc::Status status = stub_->getRange(&context, streamIdentifier, &rangeInfo);

    if (status.ok()) {
        std::cout << "Get Range successful." << std::endl;
    } else {
        std::cerr << "Error getting range: " << status.error_message() << std::endl;
    }

    return rangeInfo;
}

ivbackend::StreamIdentifierList IvBackendClient::GetAvailableUrls() {
    grpc::ClientContext context;
    ivbackend::Empty emptyRequest;
    ivbackend::StreamIdentifierList urlList;

    grpc::Status status = stub_->getAvailableUrls(&context, emptyRequest, &urlList);

    if (status.ok()) {
        std::cout << "Get Available URLs successful." << std::endl;
    } else {
        std::cerr << "Error getting available URLs: " << status.error_message() << std::endl;
    }

    return urlList;
}

bool IvBackendClient::GetLiveImages(const ivbackend::ImageRequest& request, std::function<void(const ivbackend::DataStream&)> callback) {
    grpc::ClientContext context;
    ivbackend::DataStream dataStream;

    std::unique_ptr<grpc::ClientReader<ivbackend::DataStream>> reader = stub_->getLiveImages(&context, request);

    while (!stopStreaming && reader->Read(&dataStream)) {
        callback(dataStream);
    }

    grpc::Status status = reader->Finish();

    if (status.ok()) {
        std::cout << "Get Live Images successful." << std::endl;
        return true;
    } else {
        std::cerr << "Error getting live images: " << status.error_message() << std::endl;
        return false;
    }
}

ivbackend::DataStream IvBackendClient::GetTestDataFrame() {
    grpc::ClientContext context;
    ivbackend::Empty request;
    ivbackend::DataStream dataFrame;

    // Create a client reader for receiving DataStream messages
    std::unique_ptr<grpc::ClientReader<ivbackend::DataStream>> reader(stub_->sendTestDataFrame(&context, request));

    // Receive DataStream messages from the server
    while (reader->Read(&dataFrame)) {
        if (!dataFrame.has_data() && !dataFrame.has_info()) {
            std::cout << "Received DataStream from the server." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    grpc::Status status = reader->Finish();

    if (!status.ok()) {
        std::cerr << "Error: " << status.error_message() << std::endl;
    }

    return dataFrame;
}

void IvBackendClient::StopLiveStream() {
    stopStreaming = true;
}





// TODO 
// Add method to stop 