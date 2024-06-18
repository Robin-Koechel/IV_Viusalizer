
# Add source files
file(GLOB_RECURSE SOURCE_FILES "*.cpp")

# Add UI files
set(UI_FILES
    ViewType/viewtypeframewidget.ui
    VideoPlayer/twoslidernavigator.ui
    VideoPlayer/videonavigator.ui
    VideoPlayer/videoplayerwidget.ui
    FrameData/Config/configframe.ui
    Log/logframe.ui
    main/mainwindow.ui
    GraphicalUserInterface/camframe.ui
    GraphicalUserInterface/configofregionofinterestdialog.ui
    GraphicalUserInterface/regionofinterestdialog.ui
    Export/exportdialog.ui
)

# Set the path to the Qt6 UI Compiler (uic)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ViewType)

# Generate UI headers in the specified output directory
qt6_wrap_ui(UI_HEADERS ${UI_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${UI_HEADERS})

# Add library
add_library(IV_VISUALIZER_FRONTEND_lib STATIC ${SOURCE_FILES} ${UI_HEADERS})
