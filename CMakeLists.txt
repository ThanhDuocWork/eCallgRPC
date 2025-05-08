cmake_minimum_required(VERSION 3.10)
project(eCallver2)
add_definitions("-Wl,--copy-dt-needed-entries")
set(CMAKE_CXX_STANDARD 17)

add_library(project_dependencies INTERFACE)
target_link_libraries(project_dependencies INTERFACE
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)

# Thêm các subdirectory
add_subdirectory(Utils)
add_subdirectory(Services)
add_subdirectory(Apps)