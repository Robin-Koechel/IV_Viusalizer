// #include <gtest/gtest.h>
// #include <gmock/gmock.h>
// #include <grpcpp/grpcpp.h>
// #include "MessageHandler.h" 
// #include "ivbackend.grpc.pb.h"

// // Mock gRPC service implementation for testing
// class MockIvBackendService : public ivbackend::IvBackendService::Service {
// public:
//     grpc::Status sayHello(grpc::ServerContext* context, const ivbackend::Empty* request, ivbackend::HelloResponse* response) override {
//         response->set_message("Hello, client!");
//         return grpc::Status::OK;
//     }
// };

// TEST(GrpcTest, TestSayHello) {
//     // Ensure MessageHandler is properly initialized
//     MessageHandler::getInstance();

//     // Start the gRPC server
//     std::string server_address("0.0.0.0:50051");
//     grpc::ServerBuilder builder;
//     builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
//     builder.RegisterService(MessageHandler::getInstance());
//     std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

//     // Create a channel to connect to the gRPC server
//     auto channel = grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials());
//     auto stub = ivbackend::IvBackendService::NewStub(channel);

//     // Prepare the request
//     grpc::ClientContext context;
//     ivbackend::Empty request;
//     ivbackend::HelloResponse response;

//     // Call the RPC method
//     grpc::Status status = stub->sayHello(&context, request, &response);

//     // Check if the RPC call was successful
//     ASSERT_TRUE(status.ok());
//     EXPECT_EQ(response.message(), "Hello, world!");
// }
