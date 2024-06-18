cmake_minimum_required(VERSION 3.10)
project(controller)

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

add_library(controller
    MessageConverter.cpp
    MessageHandler.cpp
)

target_include_directories(controller PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../../common 
    ${CMAKE_CURRENT_SOURCE_DIR}/../model
    ${Qt6Core_INCLUDE_DIRS} 
    ${Qt6Widgets_INCLUDE_DIRS} 
    ${Qt6Gui_INCLUDE_DIRS} 
)

target_link_libraries(controller 
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

target_link_libraries(controller
    /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so
    gRPC::grpc++
    iv_backend_proto
    iv_container_proto
    model
)

target_include_directories(controller PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR} # For the generated protobuf/grpc files
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/cmake)