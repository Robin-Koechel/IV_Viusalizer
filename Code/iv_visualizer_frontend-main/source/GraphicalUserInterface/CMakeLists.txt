find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})

add_library(GraphicalUserInterface
CamFrame.cpp
ClusterConfigDialog.cpp
ClusterTreeWidget.cpp
ConfigOfRegionOfInterestDialog.cpp
RegionOfInterestDialog.cpp
RegionOfInterestPainterScene.cpp
)        

target_include_directories(GraphicalUserInterface PUBLIC
        ../VideoPlayer
        ../FrameData
        ../Pipeline
        ../BackendStub
        ../main
        ../Export
)

target_link_libraries(GraphicalUserInterface
Qt6::Core Qt6::Widgets gRPC::grpc gRPC::grpc++ VideoPlayer FrameData
)