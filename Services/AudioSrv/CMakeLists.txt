cmake_minimum_required(VERSION 3.10)
project(AudioSrv)
add_definitions(-Wl,--copy-dt-needed-entries)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioSrv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/AudioFunc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

# 🔹 Thêm đường dẫn chứa `service.grpc.pb.h`
include_directories(${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode)

# 🔹 Thêm thư mục chứa các thư viện cần thiết
include_directories(
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

# 🔹 Tìm thư viện Protobuf và gRPC trước khi sử dụng
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 🔹 Tạo executable cho AudioSrv
add_executable(AudioSrv ${SOURCES})

# 🔹 Liên kết AudioSrv với service_proto và các thư viện cần thiết
target_link_libraries(AudioSrv
    Utils
    asound
    service_proto  # Sử dụng thư viện đã tạo từ ServiceManager
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)
