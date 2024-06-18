find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)


add_library(VideoPlayer
            AskingAfterStreamThread.cpp
            TwoSliderNavigator.cpp
            VideoNavigator.cpp
            VideoPlayer.cpp
            VideoPlayerWidget.cpp
            PainterSlider.cpp
)        

target_include_directories(VideoPlayer PUBLIC
            ../BackendConnector
            ../ViewType
            ../DataStreams
            ../main
)

target_link_libraries(VideoPlayer
    protobuf::libprotobuf
    gRPC::grpc 
    gRPC::grpc++
)
