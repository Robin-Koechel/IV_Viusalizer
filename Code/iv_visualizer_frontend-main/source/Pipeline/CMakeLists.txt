add_library(Pipeline
            AnnotationPipe.cpp
            PipelineObject.cpp
            PipelineScene.cpp
            RegionOfInterestPipe.cpp
            VideoPipe.cpp
            RegionOfInterestItem.cpp
            ColorDict.cpp
            BlackImagePipe.cpp
)        

link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)

include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)

target_include_directories(Pipeline PUBLIC
            ../FrameData
            ../Pipeline
)

target_link_libraries(Pipeline
    protobuf::libprotobuf
    gRPC::grpc 
    gRPC::grpc++
    /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so
)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/cmake)