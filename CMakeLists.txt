include(cmake/EnableVcpkg.cmake)
cmake_minimum_required(VERSION 3.22)
project(protobuf-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "include")
set(RESOURCE_DIR "resource")

find_package(Protobuf REQUIRED)
file(GLOB_RECURSE PROTO_SOURCE_FILES CONFIGURE_DEPENDS "${RESOURCE_DIR}/*.proto")

include_directories(${INCLUDE_DIR} ${SOURCE_DIR})
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "${SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PROTO_SRCS})

function(link_common_libraries target)
    target_link_libraries(${target}
            PRIVATE
            #ADD your libs here
            ${Protobuf_LIBRARIES}
    )
endfunction(link_common_libraries)

protobuf_generate(
        TARGET ${PROJECT_NAME}
        PROTOS ${PROTO_SOURCE_FILES}
        OUT_VAR PROTO_SRCS
        PROTOC_OUT_DIR ${CMAKE_CURRENT_LIST_DIR}/${SOURCE_DIR}
        APPEND_PATH
)

link_common_libraries(${PROJECT_NAME})