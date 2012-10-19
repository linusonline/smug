## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugEngine)

# Directory for library header files.
include_directories ("${PROJECT_SOURCE_DIR}/../../headers")

add_library(SmugEngine engine.c mainloop.c gameobject.c)
