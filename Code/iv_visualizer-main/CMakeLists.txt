option(BUILD_TESTS "Build the tests" ON)

cmake_minimum_required(VERSION 3.22)
project(IV_VISUALIZER)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -g")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-enum-enum-conversion")

# Find OpenCV package
find_package(OpenCV REQUIRED)

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

link_directories(/home/user/IV-Packages/scylladb-cpp-driver_2.16.x_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/scylladb-cpp-driver_2.16.x_gcc11.4.0_ubuntu2204_x64/include)

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Proto file
get_filename_component(iv_proto "ivbackend.proto" ABSOLUTE)
get_filename_component(iv_proto_path "${iv_proto}" PATH)

# Generated sources
set(iv_proto_srcs "ivbackend.pb.cc")
set(iv_proto_hdrs "ivbackend.pb.h")
set(iv_grpc_srcs "ivbackend.grpc.pb.cc")
set(iv_grpc_hdrs "ivbackend.grpc.pb.h")

add_custom_command(
      OUTPUT "${iv_proto_srcs}" "${iv_proto_hdrs}" "${iv_grpc_srcs}" "${iv_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${iv_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${iv_proto}"
      DEPENDS "${iv_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# iv_annotation_grpc_proto
add_library(iv_backend_proto
  ${iv_grpc_srcs}
  ${iv_grpc_hdrs}
  ${iv_proto_srcs}
  ${iv_proto_hdrs})
target_link_libraries(iv_backend_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})


# Proto file
get_filename_component(iv_container "ivcontainer.proto" ABSOLUTE)
get_filename_component(iv_container_path "${iv_container}" PATH)

# Generated sources
set(iv_container_srcs "ivcontainer.pb.cc")
set(iv_container_hdrs "ivcontainer.pb.h")
set(iv_container_grpc_srcs "ivcontainer.grpc.pb.cc")
set(iv_container_grpc_hdrs "ivcontainer.grpc.pb.h")

add_custom_command(
      OUTPUT "${iv_container_srcs}" "${iv_container_hdrs}" "${iv_container_grpc_srcs}" "${iv_container_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${iv_container_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${iv_container}"
      DEPENDS "${iv_container}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# iv_annotation_grpc_proto
add_library(iv_container_proto
  ${iv_container_grpc_srcs}
  ${iv_container_grpc_hdrs}
  ${iv_container_srcs}
  ${iv_container_hdrs})
target_link_libraries(iv_container_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})



include_directories(source)

add_subdirectory(source/common)
add_subdirectory(source/backend)


# Add executable
add_executable(ivbackend source/backend/ServerMain.cpp)

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

# Link the executable with libraries and dependencies
target_link_libraries(ivbackend controller common Qt6::Core Qt6::Widgets Qt6::Gui gRPC::grpc++ ${OpenCV_LIBS} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so)

# Set the default target (main executable) for the project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ivbackend)


message(STATUS "Using Protobuf ${Protobuf_VERSION}")

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)  # Ensure GoogleTest is found
    add_subdirectory(test)
endif()


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/cmake)