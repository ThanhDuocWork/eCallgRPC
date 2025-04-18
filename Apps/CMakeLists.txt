cmake_minimum_required(VERSION 3.10)
project(Apps)

add_definitions(-Wl,--copy-dt-needed-entries)

# Thêm đường dẫn của `Utils`
include_directories(
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc 
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
    ${CMAKE_SOURCE_DIR}/Utils/NlohmannJSON/inc
)

# Thêm các module con
add_subdirectory(ECallApp)
add_subdirectory(StateMachine)

# Thêm source chính (main.cpp)
set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# Tạo executable cho MainApp
add_executable(MainApp ${SOURCES})

# Liên kết `MainApp` với các thư viện con
target_link_libraries(MainApp PUBLIC
    Utils
    ECallApp
    StateMachine
    pthread
)

# Đảm bảo `Utils` được build trước
add_dependencies(MainApp Utils)
