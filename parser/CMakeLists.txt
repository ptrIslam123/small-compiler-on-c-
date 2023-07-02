cmake_minimum_required(VERSION 3.18)

project(parser)

set(SRCS
    parser.cpp
    ast_node.cpp
    base_expr_parser.cpp
)

set(HDRS
    parser.h
    ast_node.h
    base_expr_parser.h
)

add_library(${PROJECT_NAME} STATIC ${SRCS} ${HDRS})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} lexer)

add_definitions(-Wall)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
