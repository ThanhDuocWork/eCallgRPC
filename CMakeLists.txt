cmake_minimum_required(VERSION 3.10)
project(eCallver2)
add_definitions("-Wl,--copy-dt-needed-entries")
set(CMAKE_CXX_STANDARD 17)

# Thêm thư mục tìm kiếm cho header files
include_directories(${PROJECT_SOURCE_DIR}/Apps/ECallApp/inc)
include_directories(${PROJECT_SOURCE_DIR}/Apps/StateMachine/inc)
include_directories(${PROJECT_SOURCE_DIR}/Services/HMISrv/inc)
include_directories(${PROJECT_SOURCE_DIR}/Services/AudioSrv/inc)
include_directories(${PROJECT_SOURCE_DIR}/Services/MQTTSrv/inc)
include_directories(${PROJECT_SOURCE_DIR}/Utils)

# Thêm các subdirectory để build riêng từng phần
add_subdirectory(Utils)
add_subdirectory(Services)
add_subdirectory(Apps)


