## Root config file for project, collecting several build directories containing
## libs and apps to build. CMake cenerates one Makefile containing all the
## targets specified by the whole tree of CMakeLists files.

cmake_minimum_required (VERSION 2.6)
project (AllSmug)

set(MY_LIB_DIR ${PROJECT_SOURCE_DIR}/tools/lib)

include_directories ("${PROJECT_SOURCE_DIR}/2_headers")
include_directories ("${PROJECT_SOURCE_DIR}/tools/include")

## Build the sub-projects in these directories. Each has its own CMakeLists.txt.
add_subdirectory (1_src/audio)
add_subdirectory (1_src/utils)
add_subdirectory (1_src/graphics)
add_subdirectory (1_src/platform)
add_subdirectory (1_src/input)
add_subdirectory (1_src/engine)
add_subdirectory (4_test)
add_subdirectory (3_guitest)

file(COPY "${PROJECT_SOURCE_DIR}/5_res" DESTINATION "${PROJECT_BINARY_DIR}")
