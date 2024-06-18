find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(DataStreams
            DataFrameRingBuffer.cpp
            DataFrameStream.cpp
            DataStream.cpp
            IvBackendStream.cpp
)        

target_include_directories(DataStreams PUBLIC
            ../FrameData
            ../BackendConnector
)

target_link_libraries(DataStreams
    protobuf::libprotobuf
    gRPC::grpc 
    gRPC::grpc++
)
