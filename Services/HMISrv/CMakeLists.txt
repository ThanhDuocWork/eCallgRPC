cmake_minimum_required(VERSION 3.10)
project(HMISrv)
add_definitions(-Wl,--copy-dt-needed-entries)
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HMISrv.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/HMISrvFunc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
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

# 🔹 Tạo executable cho HMISrv
add_executable(HMISrv ${SOURCES})

# 🔹 Liên kết HMISrv với service_proto và các thư viện cần thiết
target_link_libraries(HMISrv
    Utils
    service_proto  # Sử dụng thư viện đã tạo từ ServiceManager
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)
