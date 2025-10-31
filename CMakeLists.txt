cmake_minimum_required(VERSION 3.22)
project(interval_heap LANGUAGES CXX)

add_library(
    ${PROJECT_NAME}
    INTERFACE
    "include/interval_heap.hpp"
    "include/tracker.hpp"
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)
target_include_directories(${PROJECT_NAME} INTERFACE "include")

add_executable(${PROJECT_NAME}_test "src/main.cpp")

target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})
