cmake_minimum_required(VERSION 2.8)

project(benchmark CXX C)
#cmake_policy(SET CMP0012 NEW)
#cmake_policy(SET CMP0048 OLD)

set (PROJECT_VERSION_MAJOR 1) 
set (PROJECT_VERSION_MINOR 0)
set (PROJECT_VERSION_PATCH 0)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include(${PROJECT_SOURCE_DIR}/cmake/UserOption.cmake)
#include(${PROJECT_SOURCE_DIR}/cmake/CheckCompiler.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Checkheaders.cmake)

add_subdirectory(src)
add_subdirectory(examples)


include(${PROJECT_SOURCE_DIR}/package/package.cmake)