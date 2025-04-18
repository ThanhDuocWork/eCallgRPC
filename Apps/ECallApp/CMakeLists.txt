cmake_minimum_required(VERSION 3.10)
project(ECallApp)

add_definitions(-Wl,--copy-dt-needed-entries)

# Thêm đường dẫn đến `Utils`
include_directories(
    ${CMAKE_SOURCE_DIR}/Apps/ECallApp/inc
    ${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc 
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
    ${CMAKE_SOURCE_DIR}/Utils/NlohmannJSON/inc
)

include_directories(
    ${CMAKE_SOURCE_DIR}/Apps/ECallApp/inc
    ${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode
)

# Thêm source files
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HMIAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MQTTAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECallApp.cpp
)

# Tạo thư viện ECallApp
add_library(ECallApp STATIC ${SOURCES})

# Liên kết với các thư viện cần thiết
target_link_libraries(ECallApp PUBLIC
    Utils
    service_proto
    ${Protobuf_LIBRARIES}
    ${gRPC_LIBRARIES}
    pthread
)
