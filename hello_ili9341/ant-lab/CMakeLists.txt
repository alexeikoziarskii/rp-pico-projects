cmake_minimum_required(VERSION 3.13)

project(ant-lab)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PUBLIC
    "${PROJECT_SOURCE_DIR}/include/ant-lab/ant-lab.h"
    "${PROJECT_SOURCE_DIR}/src/ant-lab.c"
)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/../ili9341/include")