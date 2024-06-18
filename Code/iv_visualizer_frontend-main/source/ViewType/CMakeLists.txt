add_library(ViewType
            AnnotationAndVideo.cpp
            AnnotationWithoutVideo.cpp
            OnlyVideo.cpp
            ViewType.cpp
            ViewTypeFrame.cpp
            ViewTypeFrameWidget.cpp
)        

target_include_directories(ViewType PUBLIC
            ../FrameData
            ../Pipeline
)

target_link_libraries(ViewType

)
