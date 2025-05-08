cmake_minimum_required(VERSION 3.10)
project(Apps)

add_definitions(-Wl,--copy-dt-needed-entries)


# module subdirectory
add_subdirectory(ECallApp)
add_subdirectory(StateMachine)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# create executable
add_executable(MainApp ${SOURCES})

target_include_directories(MainApp PUBLIC
    ${CMAKE_SOURCE_DIR}/Apps/ECallApp/inc
    ${CMAKE_SOURCE_DIR}/Apps/StateMachine/inc
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
    ${CMAKE_SOURCE_DIR}/Utils/NlohmannJSON/inc
    ${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode
)

# linking lib
target_link_libraries(MainApp PUBLIC
    Utils
    ECallApp
    StateMachine
    pthread
)

# make sure `Utils` build before
add_dependencies(MainApp Utils)
