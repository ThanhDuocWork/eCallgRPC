cmake_minimum_required(VERSION 3.10)
project(MQTTSrv)
add_definitions(-Wl,--copy-dt-needed-entries)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MQTTSrv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MQTTFunc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/HMQTTSrv.h
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/MQTTFunc.h
)

# 🔹 Thêm đường dẫn chứa `service.grpc.pb.h`
include_directories(${CMAKE_SOURCE_DIR}/Services/ServiceManager/gencode)

# 🔹 Thêm thư mục chứa các thư viện cần thiết
include_directories(
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
    ${CMAKE_SOURCE_DIR}/Utils/CANBus/inc
)

# 🔹 Tìm thư viện Protobuf và gRPC trước khi sử dụng
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 🔹 Tạo executable cho MQTT
add_executable(MQTTSrv ${SOURCES})

# 🔹 Liên kết MQTTSrv với service_proto và các thư viện cần thiết
target_link_libraries(MQTTSrv
    service_proto  # Sử dụng thư viện đã tạo từ ServiceManager
    mosquitto
    Utils
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)
