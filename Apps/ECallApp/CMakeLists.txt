cmake_minimum_required(VERSION 3.10)
project(ECallApp)

add_definitions(-Wl,--copy-dt-needed-entries)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HMIAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MQTTAdapter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Reconnect.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ECallApp.cpp
)

# create ECallApp
add_library(ECallApp STATIC ${SOURCES})

target_include_directories(ECallApp PUBLIC
    ${CMAKE_SOURCE_DIR}/Apps/ECallApp/inc
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc
)

# linking lib
target_link_libraries(ECallApp PUBLIC
    Utils
    service_proto
    ${Protobuf_LIBRARIES}
    ${gRPC_LIBRARIES}
    pthread
)
