cmake_minimum_required(VERSION 3.10)
project(StateMachine)

add_definitions(-Wl,--copy-dt-needed-entries)

#Thêm đường dẫn đến `Utils`
include_directories(
    ${CMAKE_SOURCE_DIR}/StateMachine/inc
    ${CMAKE_SOURCE_DIR}/ECallApp/inc
    ${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc  
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
    ${CMAKE_SOURCE_DIR}/Utils/NlohmannJSON/inc
)

include_directories(
    ${CMAKE_SOURCE_DIR}/StateMachine/inc
    ${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode
)

# Thêm các source file của StateMachine
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CancelTimerState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/CenterState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EventTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/IDLEState.cpp
)

# Tạo thư viện StateMachine
add_library(StateMachine STATIC ${SOURCES})

# Liên kết với các thư viện cần thiết
target_link_libraries(StateMachine PUBLIC
    Utils
    ECallApp
    service_proto
    ${Protobuf_LIBRARIES}
    ${gRPC_LIBRARIES}
    pthread
)
