# cmake_minimum_required(VERSION 3.10)

# # Include the Google Test framework
# find_package(GTest REQUIRED)

# find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
# include_directories(${Qt6Core_INCLUDE_DIRS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS})


# link_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib)
# include_directories(/home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/include)


# # Optionally group all your test source files into a variable
# file(GLOB UNIT_TEST_SOURCES "*.cpp")

# # Create an executable for your tests
# add_executable(unit_tests ${UNIT_TEST_SOURCES})


# # Include directories from the main code that the tests need access to
# target_include_directories(${BINARY} PRIVATE 
#     ${CMAKE_SOURCE_DIR}/source/BackendConnector
#     ${CMAKE_SOURCE_DIR}/source/BackendStub
#     ${CMAKE_SOURCE_DIR}/source/DataStreams
#     ${CMAKE_SOURCE_DIR}/source/Export
#     ${CMAKE_SOURCE_DIR}/source/ExportSettings
#     ${CMAKE_SOURCE_DIR}/source/FrameData
#     ${CMAKE_SOURCE_DIR}/source/GraphicalUserInterface
#     ${CMAKE_SOURCE_DIR}/source/Log
#     ${CMAKE_SOURCE_DIR}/source/main
#     ${CMAKE_SOURCE_DIR}/source/Pipeline
#     ${CMAKE_SOURCE_DIR}/source/VideoPlayer
#     ${CMAKE_SOURCE_DIR}/source/ViewType
# )

# # Link against Google Test and your project's libraries
# target_link_libraries(unit_tests PRIVATE GTest::GTest GTest::Main common controller model /home/user/IV-Packages/2d-annotation-handler_0.2.1_gcc11.4.0_ubuntu2204_x64/lib/libiv_2d_annotation_handler.so)
# enable_testing()
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} /home/user/IV-Packages/2d-annotation-handler_0.2.0_gcc11.4.0_ubuntu2204_x64/cmake)
