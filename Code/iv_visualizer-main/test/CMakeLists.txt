set(BINARY ${CMAKE_PROJECT_NAME}_test)

# Add coverage flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Create a list of test source files
file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.cpp)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.0_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.0_gcc11.4.0_ubuntu2204_x64/include)

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Add subdirectories that contain the test source files
add_subdirectory(unit_tests)

# Add an executable for the tests
add_executable(${BINARY} ${TEST_SOURCES})

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)
# Include OpenCV directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Include directories from the main code that the tests need access to
target_include_directories(${BINARY} PRIVATE 
    ${CMAKE_SOURCE_DIR}/source/common
    ${CMAKE_SOURCE_DIR}/source/backend/controller
    ${CMAKE_SOURCE_DIR}/source/backend/model
)

# Link against the libraries used by your project and GoogleTest
target_link_libraries(${BINARY} PRIVATE common controller model ${OpenCV_LIBS} iv_2d_annotation_handler GTest::GTest GTest::Main)

add_test(NAME ${BINARY} COMMAND ${BINARY})
