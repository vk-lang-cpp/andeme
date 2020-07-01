cmake_minimum_required(VERSION 3.10)

project(Andeme)

if (WIN32)
add_definitions(/std:c++17 /D_WIN32_WINNT=0x600)
else ()
add_definitions(-std=c++17 -fPIC)
endif()

set(THIRDPARTY_PATH ${CMAKE_SOURCE_DIR}/thirdparty)

set(QT_PATH ${THIRDPARTY_PATH}/qt5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(protobuf REQUIRED PATHS ${THIRDPARTY_PATH}/grpc NO_DEFAULT_PATH)
find_package(Threads REQUIRED)
                              
find_package(Qt5 COMPONENTS Widgets REQUIRED PATHS ${THIRDPARTY_PATH}/qt5)
find_package(gRPC CONFIG REQUIRED PATHS ${THIRDPARTY_PATH}/grpc)
find_package(GTest REQUIRED PATHS ${THIRDPARTY_PATH}/gtest)


find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin PATHS ${THIRDPARTY_PATH}/grpc/bin)
if(NOT GRPC_CPP_PLUGIN)
    message(FATAL_ERROR "System grpc_cpp_plugin not found")
endif()

# Build schema

file(GLOB ANDEME_SCHEMA_PROTO schema/*.proto)
file(GLOB ANDEME_SERVICES_PROTO schema/services/*.proto)

include_directories("${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_SOURCE_DIR}")

# Generated schema

add_library(schema STATIC)

protobuf_generate(
    LANGUAGE cpp
    TARGET schema
    PROTOS ${ANDEME_SCHEMA_PROTO} ${ANDEME_SERVICES_PROTO}
    )

foreach(SVC_PROTO ${ANDEME_SERVICES_PROTO})
    get_filename_component(SVC_NAME ${SVC_PROTO} NAME_WE)
    set(${SVC_NAME}_CPP
        ${CMAKE_CURRENT_BINARY_DIR}/schema/services/${SVC_NAME}.grpc.pb.cc)
    set(${SVC_NAME}_H
        ${CMAKE_CURRENT_BINARY_DIR}/schema/services/${SVC_NAME}.grpc.pb.h)

    add_custom_command(
        OUTPUT
        "${${SVC_NAME}_H}"
        "${${SVC_NAME}_CPP}"
        BYPRODUCTS
        "${${SVC_NAME}_H}"
        "${${SVC_NAME}_CPP}"
          COMMAND protobuf::protoc
          ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}" -I "${CMAKE_SOURCE_DIR}"
            --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN}"
            "${SVC_PROTO}"
            DEPENDS "${SVC_PROTO}")
    list(APPEND SERVICES_SRCS
        "${${SVC_NAME}_H}"
        "${${SVC_NAME}_CPP}")
endforeach()
target_sources(schema PUBLIC ${SERVICES_SRCS})
target_include_directories(schema PUBLIC $<TARGET_PROPERTY:gRPC::grpc++,INTERFACE_INCLUDE_DIRECTORIES>)

# client library
file(GLOB ANDEME_CLIENT_SRCS client/*.cpp)
add_library(AndemeClient ${ANDEME_CLIENT_SRCS})
target_include_directories(AndemeClient PRIVATE client)
target_link_libraries(AndemeClient schema gRPC::grpc++_unsecure)


# server application

file(GLOB ANDEME_SERVER_SRCS server/*.cpp)
add_executable(AndemeServer ${ANDEME_SERVER_SRCS})
target_include_directories(AndemeServer PRIVATE schema server)
target_link_libraries(AndemeServer schema gRPC::grpc++_unsecure)

# AndemeQtGui
file(GLOB ANDEME_QT_GUI_SRCS qtgui/*.cpp qtgui/*.h)
file(GLOB ANDEME_QT_GUI_UI qtgui/*.ui)

qt5_wrap_ui(ANDEME_QT_GUI_SRCS ${ANDEME_QT_GUI_UI})

add_executable(AndemeQtGui ${ANDEME_QT_GUI_SRCS})
target_link_libraries(AndemeQtGui Qt5::Widgets AndemeClient)
target_include_directories(AndemeQtGui PRIVATE qtgui)
