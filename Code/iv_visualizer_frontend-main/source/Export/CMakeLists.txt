find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(OpenCV REQUIRED)

add_library(Export
            ExportDialog.cpp
            ImageToVideoConverter.cpp
            Encoder.cpp
)        

set(OpenCV_DIR /home/user/IV-Packages/opencv_4.8.1_gcc11.4.0_ubuntu2204_x64/lib/cmake/opencv4)
include_directories(/usr/include/x86_64-linux-gnu)
include_directories(${OpenCV_INCLUDE_DIRS})

target_include_directories(Export PUBLIC
            ../VideoPlayer
            ../BackendConnector
            ../DataStreams
            ../Pipeline
            ../FrameData
)

target_link_libraries(Export
            swscale
            avcodec
            avutil
            avformat
            protobuf::libprotobuf 
            gRPC::grpc 
            gRPC::grpc++
            ${OpenCV_LIBS}
)
