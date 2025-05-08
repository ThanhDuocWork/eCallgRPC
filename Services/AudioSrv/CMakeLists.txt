cmake_minimum_required(VERSION 3.10)
project(AudioSrv)
add_definitions(-Wl,--copy-dt-needed-entries)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioSrv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioFunc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
# create executable
add_executable(AudioSrv ${SOURCES})

target_include_directories(AudioSrv PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
)

# find lib Protobuf and gRPC
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# linking lib 
target_link_libraries(AudioSrv
    Utils
    asound
    service_proto  
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)
