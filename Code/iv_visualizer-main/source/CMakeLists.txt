set(BINARY ${CMAKE_PROJECT_NAME})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

add_executable(${BINARY}_run ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS})
target_link_libraries(${BINARY}_run Qt6::Core Qt6::Gui Qt6::Widgets gRPC::grpc++)

message("Qt6Core_VERSION: ${Qt6Core_VERSION}")
message("Qt6Gui_VERSION: ${Qt6Gui_VERSION}")
message("Qt6Widgets_VERSION: ${Qt6Widgets_VERSION}")

