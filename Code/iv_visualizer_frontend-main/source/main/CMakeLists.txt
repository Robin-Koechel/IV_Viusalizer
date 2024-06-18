# Find Protobuf and gRPC
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

add_library(main
            MainWindow.cpp
            main.cpp
            ${PROTO_SOURCES}
            ${GRPC_SOURCES}
)        

target_include_directories(main PUBLIC
            ../VideoPlayer
            ../BackendStub
            ../BackendConnector
            ../ExportSettings
            ../GraphicalUserInterface
)

target_link_libraries(main
            protobuf::libprotobuf
            gRPC::grpc
            gRPC::grpc++
            IvBackendClient     
            Qt6::Core Qt6::Widgets 
            GraphicalUserInterface
            Log
            VideoPlayer
            ViewType
            FrameData
)
