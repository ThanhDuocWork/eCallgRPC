cmake_minimum_required(VERSION 3.0)
project(Utils)

set(
    SOURCES
    Logger/src/Logger.cpp
    FeatureTimer/src/FeatureTimer.cpp
    SimulateButton/src/SimulateButton.cpp
    ManagerMessage/src/ManagerMessage.cpp
    EventData/src/EventData.cpp
    CANBus/src/CanBus.cpp
)
#!!
add_definitions(-D_STD_LOG_)

add_library(Utils SHARED ${SOURCES})

# Thêm các thư mục include
target_include_directories(
    Utils PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/FeatureTimer/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/NlohmannJSON/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/SimulateButton/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/ManagerMessage/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/EventData/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/CANBus/inc
)


install(TARGETS Utils DESTINATION ${CMAKE_SOURCE_DIR})
