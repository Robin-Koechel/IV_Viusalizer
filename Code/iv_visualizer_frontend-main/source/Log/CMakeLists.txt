add_library(Log
            Log.cpp
            LogDatabaseConnector.cpp
            LogFrame.cpp
            LogDbException.cpp
)        

target_include_directories(Log PUBLIC
    ../main
)

target_link_libraries(Log
    protobuf::libprotobuf
    gRPC::grpc 
    gRPC::grpc++
)

