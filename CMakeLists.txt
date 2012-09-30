## Root config file for project, collecting several build directories containing
## libs and apps to build. CMake cenerates one Makefile containing all the
## targets specified by the whole tree of CMakeLists files.

cmake_minimum_required (VERSION 2.6)
project (AllSmug)

## Build the sub-projects in these directories. Each has its own CMakeLists.txt.
add_subdirectory (src/utils)
add_subdirectory (src/graphics)
add_subdirectory (src/platform)
add_subdirectory (test)
add_subdirectory (guitest)
