cmake_minimum_required(VERSION 3.0)

project(model)

# Add source files to the library
add_library(model 
    DataManager.cpp
    DBManager.cpp
    Encoder.cpp
    FSManager.cpp
)

# Specify include directories
target_include_directories(
    model PUBLIC
    ../../common
    ../model
    ${OpenCV_INCLUDE_DIRS}
)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Link the model library with OpenCV
target_link_libraries(model ${OpenCV_LIBS})

# Link other libraries
link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

link_directories(/home/user/IV-Packages/scylladb-cpp-driver_2.16.x_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/scylladb-cpp-driver_2.16.x_gcc11.4.0_ubuntu2204_x64/include)

target_link_libraries(model
    common
    iv_2d_annotation_handler
    scylla-cpp-driver
)

# Use pkg-config to get hints for remaining libraries
find_package(PkgConfig REQUIRED)

# GLib
pkg_check_modules(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
target_link_libraries(model ${GLIB_LIBRARIES})

# Cairo
#pkg_check_modules(CAIRO REQUIRED cairo)
#include_directories(${CAIRO_INCLUDE_DIRS})
#link_directories(${CAIRO_LIBRARY_DIRS})
#target_link_libraries(model ${CAIRO_LIBRARIES})

# GDK Pixbuf
pkg_check_modules(GDK_PIXBUF REQUIRED gdk-pixbuf-2.0)
include_directories(${GDK_PIXBUF_INCLUDE_DIRS})
link_directories(${GDK_PIXBUF_LIBRARY_DIRS})
target_link_libraries(model ${GDK_PIXBUF_LIBRARIES})

# librsvg
pkg_check_modules(LIBRSVG REQUIRED librsvg-2.0)
include_directories(${LIBRSVG_INCLUDE_DIRS})
link_directories(${LIBRSVG_LIBRARY_DIRS})
target_link_libraries(model ${LIBRSVG_LIBRARIES})

# FFmpeg/libav
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWSCALE REQUIRED libswscale)
include_directories(${AVCODEC_INCLUDE_DIRS} ${AVFORMAT_INCLUDE_DIRS} ${AVUTIL_INCLUDE_DIRS} ${SWSCALE_INCLUDE_DIRS})
link_directories(${AVCODEC_LIBRARY_DIRS} ${AVFORMAT_LIBRARY_DIRS} ${AVUTIL_LIBRARY_DIRS} ${SWSCALE_LIBRARY_DIRS})
target_link_libraries(model ${AVCODEC_LIBRARIES} ${AVFORMAT_LIBRARIES} ${AVUTIL_LIBRARIES} ${SWSCALE_LIBRARIES})