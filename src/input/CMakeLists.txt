## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugInput)

# Directory for library header files.
include_directories ("${PROJECT_SOURCE_DIR}/../../headers")

# Add a target to build the library "SmugUtils" from the specified sources.
add_library(SmugInput controller.c input.c)
target_link_libraries(SmugInput SmugUtils)
