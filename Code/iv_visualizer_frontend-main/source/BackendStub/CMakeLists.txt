link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

add_library(BackendStub
            DataStreamMock.cpp
            MockReader.cpp
            MockWriter.cpp
)        

target_include_directories(BackendStub PUBLIC
            ../FrameData
            ../ViewType
            ../DataStreams
            ../BackendConnector
)

target_link_libraries(BackendStub
    /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so
)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/cmake)
