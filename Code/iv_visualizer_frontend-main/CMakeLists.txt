cmake_minimum_required(VERSION 3.10)

project(IV_VISUALIZER_FRONTEND)
option(BUILD_TESTS "Build the tests" ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Enable C++ code coverage flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(Protobuf CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")
link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)
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
include_directories(${OpenCV_INCLUDE_DIRS})
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


# Find Qt6 package
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

# Include directories for Qt6
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS})
link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)

include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

# Add subdirectories 
add_subdirectory(source/BackendConnector)
add_subdirectory(source/BackendStub)
add_subdirectory(source/DataStreams)
add_subdirectory(source/FrameData)
add_subdirectory(source/Log)
add_subdirectory(source/Pipeline)
add_subdirectory(source/VideoPlayer)
add_subdirectory(source/ExportSettings)
add_subdirectory(source/Export)
add_subdirectory(source/ViewType)
add_subdirectory(source/GraphicalUserInterface)
add_subdirectory(source/main)

# Create a list of source files using globbing
file(GLOB_RECURSE SOURCE_FILES LIST_DIRECTORIES false "source/*.cpp")

# Add an executable for the main application
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link your executable with the required libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    protobuf::libprotobuf
    gRPC::grpc 
    gRPC::grpc++
    IvBackendClient
    /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so
)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
# Link your executable with the required libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    protobuf::libprotobuf
    gRPC::grpc 
    gRPC::grpc++
    IvBackendClient
    /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/protobuf  # Include the directory containing the generated Protobuf files
)

# Ensure protos target is built first
#add_dependencies(${PROJECT_NAME} protos)

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)  # Ensure GoogleTest is found
    add_subdirectory(tests)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/cmake)