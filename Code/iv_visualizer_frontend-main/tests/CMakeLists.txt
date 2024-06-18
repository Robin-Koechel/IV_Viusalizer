# tests/CMakeLists.txt

set(BINARY ${CMAKE_PROJECT_NAME}_test)

# Add coverage flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

# Include the Google Test framework
find_package(GTest REQUIRED)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})

# Create a list of test source files
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cpp)

# Add subdirectories that contain the test source files
#add_subdirectory(unitTests)


# Add an executable for the tests
add_executable(${BINARY} ${TEST_SOURCES})


# Include directories from the main code that the tests need access to
target_include_directories(${BINARY} PRIVATE 
    ${CMAKE_SOURCE_DIR}/source/BackendConnector
    ${CMAKE_SOURCE_DIR}/source/BackendStub
    ${CMAKE_SOURCE_DIR}/source/DataStreams
    ${CMAKE_SOURCE_DIR}/source/Export
    ${CMAKE_SOURCE_DIR}/source/ExportSettings
    ${CMAKE_SOURCE_DIR}/source/FrameData
    ${CMAKE_SOURCE_DIR}/source/GraphicalUserInterface
    ${CMAKE_SOURCE_DIR}/source/Log
    ${CMAKE_SOURCE_DIR}/source/main
    ${CMAKE_SOURCE_DIR}/source/Pipeline
    ${CMAKE_SOURCE_DIR}/source/VideoPlayer
    ${CMAKE_SOURCE_DIR}/source/ViewType
)

# Link your main project and required libraries
target_link_libraries(${BINARY} PRIVATE Qt6::Core Qt6::Widgets gRPC::grpc gRPC::grpc++ GTest::GTest GTest::Main main IvBackendClient BackendStub DataStreams ExportSettings FrameData GraphicalUserInterface Export Log VideoPlayer ViewType Pipeline)
enable_testing()
# Add the test to the test suite
add_test(NAME ${BINARY} COMMAND ${BINARY})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.0_gcc11.4.0_ubuntu2204_x64/cmake)