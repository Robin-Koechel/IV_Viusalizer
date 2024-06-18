cmake_minimum_required(VERSION 3.10)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)

include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

# Add the generated files to the library target
add_library(IvBackendClient STATIC
    GrpcReader.cpp
    GrpcWriter.cpp
    InitialisationMessage.cpp
    Reader.cpp
    Stream.cpp
    TimeRange.cpp
    Writer.cpp
    MessageConverter.cpp
    IvBackendClient.cpp
)

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)

include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

# Find Qt6 package
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Include directories for Qt6
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS})

# Link your executable with the required libraries
target_link_libraries(IvBackendClient Qt6::Core Qt6::Widgets protobuf::libprotobuf 
gRPC::grpc 
gRPC::grpc++
iv_backend_proto
/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so
)

# Specify include directories
target_include_directories(IvBackendClient PUBLIC
    ../FrameData
    ../ViewType
    ../DataStreams
    ../BackendStub
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/cmake)