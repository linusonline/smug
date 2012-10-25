## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugEngineTest)

# Header files for Smug library.
include_directories ("${PROJECT_SOURCE_DIR}/../../headers")
# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/../headers")

add_library(SmugEngineTest bodytest.c gameobjecttest.c)

target_link_libraries (SmugEngineTest SmugEngine SmugUtils SmugGraphics SmugPlatform)
