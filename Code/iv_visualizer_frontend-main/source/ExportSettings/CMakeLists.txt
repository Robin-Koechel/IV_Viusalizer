find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(ExportSettings
            JsonSettingsSerializer.cpp
)        

target_include_directories(ExportSettings PUBLIC
            ../FrameData
            ../BackendConnector
            ../main
)

target_link_libraries(ExportSettings
protobuf::libprotobuf 
gRPC::grpc 
gRPC::grpc++
)