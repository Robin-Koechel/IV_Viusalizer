cmake_minimum_required(VERSION 3.22)

project(dummy)

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
get_filename_component(iv_container "ivcontainer.proto" ABSOLUTE)
get_filename_component(iv_container_path "${iv_container}" PATH)

# Find Qt6 package
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Include directories for Qt6
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS})

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

# Add the generated files to the target
add_executable(${PROJECT_NAME} dummy.cpp)

# Link gRPC libraries
target_link_libraries(${PROJECT_NAME} gRPC::grpc++ iv_container_proto Qt6::Core
    Qt6::Gui)

# Link other required libraries
target_link_libraries(${PROJECT_NAME} protobuf::libprotobuf)

# Include generated files and project source directory
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})