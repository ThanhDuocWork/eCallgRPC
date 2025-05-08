cmake_minimum_required(VERSION 3.10)
project(StateMachine)

add_definitions(-Wl,--copy-dt-needed-entries)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CancelTimerState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CenterState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EventTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IDLEState.cpp
)

# create lib STATIC StateMachine
add_library(StateMachine STATIC ${SOURCES})

target_include_directories(StateMachine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/Apps/ECallApp/inc
)
target_link_libraries(StateMachine PUBLIC
    Utils
    ECallApp
    service_proto
    ${Protobuf_LIBRARIES}
    ${gRPC_LIBRARIES}
    pthread
)
