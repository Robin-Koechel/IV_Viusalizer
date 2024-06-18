add_library(FrameData
            Config/Config.cpp
            Config/ConfigContainer.cpp
            Config/ConfigDetector.cpp
            Config/ConfigFrame.cpp
            Config/ConfigGStreamer.cpp
            Data/BoundingBox.cpp
            Data/DataFrame.cpp
            Data/Image.cpp
            Data/PipelineData.cpp
            Data/RegionOfInterest.cpp
            
)        

target_include_directories(FrameData PUBLIC
            ../FrameData
            ../Pipeline
)

target_link_libraries(FrameData

)
