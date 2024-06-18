#include <grpcpp/grpcpp.h>
#include "controller/MessageHandler.h"

void RunServer() {
    std::string server_address("0.0.0.0:50051");

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());

    MessageHandler* service = MessageHandler::getInstance();

    // Use getOrCreateChannel to get or create a channel
    std::shared_ptr<grpc::Channel> channel = service->getOrCreateChannel("dummy:50052");

    if (channel) {
        // Access the URL directly from the stored pair
        std::cout << "IVBackend successfully created a channel to Processing Tools." << std::endl;
        std::cout << "Channel target: " << service->getChannelUrl(channel) << std::endl;

        // Create a gRPC stub using the channel.
        std::unique_ptr<ivcontainerpipeline::IvService::Stub> stub = ivcontainerpipeline::IvService::NewStub(channel);

        // Create an Empty message as input for the Init RPC.
        ivcontainerpipeline::Empty request;

        // Create a context for the RPC call.
        grpc::ClientContext context;

        // Make the RPC call to Init.
        ivcontainerpipeline::ConfigResponseMessage response;
        grpc::Status status = stub->Init(&context, request, &response);

        // Check if the RPC call was successful.
        if (status.ok()) {
            // Print the received configuration or handle it as needed.
            // For example:
            std::cout << "Init RPC successful. Received configuration:\n" << response.DebugString() << std::endl;
        } else {
            // Handle RPC error.
            std::cerr << "Init RPC failed. Error: " << status.error_message() << std::endl;
            // TODO Handle the error as needed
            return;
        }
    } else {
        std::cerr << "Error creating channel to Server B." << std::endl;
        // TODO Handle the error as needed
        return;
    }

    builder.RegisterService(service);
    builder.SetMaxReceiveMessageSize(-1);

    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    server->Wait();
}


int main(int argc, char** argv) {
    RunServer();
    return 0;
}
