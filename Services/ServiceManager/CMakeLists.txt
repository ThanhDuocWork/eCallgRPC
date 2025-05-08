cmake_minimum_required(VERSION 3.19)
project(ServiceManager)
add_definitions(-Wl,--copy-dt-needed-entries)
set(CMAKE_CXX_STANDARD 14)

# ================================
# Add the necessary libraries
# ================================
find_package(Threads REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

message(STATUS "Using protobuf ${Protobuf_VERSION}")
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_GRPC_GRPCPP gRPC::grpc++)

if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# ================================
# Xử lý ProtoBuf & gRPC
# ================================

# define dir contain file .proto
set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(GENERATED_PROTO_DIR ${CMAKE_BINARY_DIR}/generated)

# Make sure the output directory exists
file(MAKE_DIRECTORY ${GENERATED_PROTO_DIR})

# define file .proto
set(PROTO_FILES ${PROTO_SRC_DIR}/service.proto)

# create file .pb.cc và .grpc.pb.cc
add_custom_command(
    OUTPUT ${GENERATED_PROTO_DIR}/service.pb.cc
           ${GENERATED_PROTO_DIR}/service.grpc.pb.cc
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --proto_path=${PROTO_SRC_DIR}
         --grpc_out=${GENERATED_PROTO_DIR}
         --cpp_out=${GENERATED_PROTO_DIR}
         --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE}
         ${PROTO_FILES}
    DEPENDS ${PROTO_FILES}
)

# create lib service_proto
add_library(service_proto STATIC
    ${GENERATED_PROTO_DIR}/service.pb.cc
    ${GENERATED_PROTO_DIR}/service.grpc.pb.cc
)

target_include_directories(service_proto PUBLIC ${GENERATED_PROTO_DIR})
target_link_libraries(service_proto ${_GRPC_GRPCPP} ${_PROTOBUF_LIBPROTOBUF})
target_include_directories(service_proto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

# create executable for ServiceManager
# ================================
add_executable(ServiceManager src/ServiceManager.cpp)

# ================================
# lib necessary PUBLIC
target_include_directories(ServiceManager PUBLIC
    ${CMAKE_SOURCE_DIR}/Apps/ECallApp/inc
    ${CMAKE_SOURCE_DIR}/Utils/Parameter/inc
    ${CMAKE_SOURCE_DIR}/Utils/Logger/inc
)
# ================================

# Link lib `service_proto` và `Threads`
target_link_libraries(ServiceManager PUBLIC
    ECallApp
    Utils
    service_proto
    Threads::Threads
)
target_include_directories(ServiceManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)

