cmake_minimum_required(VERSION 3.10)
project(HMISrv)
add_definitions(-Wl,--copy-dt-needed-entries)
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HMISrv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HMISrvFunc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# create executable
add_executable(HMISrv ${SOURCES})

target_include_directories(HMISrv PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
)

# find lib Protobuf and gRPC
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# linking lib 
target_link_libraries(HMISrv
    Utils
    service_proto  # Sử dụng thư viện đã tạo từ ServiceManager
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)
