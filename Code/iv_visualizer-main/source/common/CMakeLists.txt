cmake_minimum_required(VERSION 3.10)
project(common)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

# Add all source files to the library
add_library(common
    ConfigContainer.cpp
    ConfigContainer.h
    ConfigDetector.cpp
    ConfigDetector.h
    ConfigGStreamer.cpp
    ConfigGStreamer.h
    Config.cpp
    Config.h
    DataFrame.cpp
    DataFrame.h
    DbConfig.cpp
    DbConfig.h
    Image.cpp
    Image.h
    Mask.cpp
    Mask.h
    TimeRange.cpp
    TimeRange.h
    TimeRangeVector.cpp
    TimeRangeVector.h
)

# Include directories
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(common PRIVATE 
    ${Qt6Core_INCLUDE_DIRS} 
    ${Qt6Widgets_INCLUDE_DIRS} 
    ${Qt6Gui_INCLUDE_DIRS} 
)

target_link_libraries(common 
    /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so
    Qt6::Core 
    Qt6::Gui 
    Qt6::Widgets 
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/cmake)