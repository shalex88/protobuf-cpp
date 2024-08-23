include(cmake/SetVersion.cmake)
include(cmake/EnableVcpkg.cmake)
cmake_minimum_required(VERSION 3.22)
project(protobuf-cpp LANGUAGES CXX VERSION ${VERSION})

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")

# Set variables for protobuf usage
set(PROTO_DIR "proto")
set(GENERATED_DIR "generated")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${GENERATED_DIR})
find_package(absl REQUIRED)
find_package(Protobuf REQUIRED)
file(GLOB_RECURSE PROTO_FILES CONFIGURE_DEPENDS "${PROTO_DIR}/*.proto")

include_directories(${INCLUDE_DIR} ${SOURCE_DIR} ${CMAKE_BINARY_DIR}/${GENERATED_DIR} ${Protobuf_INCLUDE_DIRS})
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PROTO_SOURCE_FILES})

function(link_common_libraries target)
    target_link_libraries(${target}
            PRIVATE
            #ADD your libs here
            absl::base
            protobuf::libprotoc
            protobuf::libprotobuf
    )
endfunction(link_common_libraries)

protobuf_generate(
        TARGET ${PROJECT_NAME}
        PROTOS ${PROTO_FILES}
        OUT_VAR PROTO_SOURCE_FILES
        PROTOC_OUT_DIR ${CMAKE_BINARY_DIR}/${GENERATED_DIR}
)

link_common_libraries(${PROJECT_NAME})