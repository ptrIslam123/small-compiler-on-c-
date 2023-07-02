cmake_minimum_required(VERSION 3.18)

project(small_clang)

add_subdirectory(lexer)
add_subdirectory(parser)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} lexer parser)

add_definitions(-Wall)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
