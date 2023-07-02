cmake_minimum_required(VERSION 3.18)

project(lexer)

set(SRCS token.cpp lexer.cpp)
set(HDRS token.h lexer.h)

add_library(${PROJECT_NAME} STATIC ${SRCS} ${HDRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

add_definitions(-Wall)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)