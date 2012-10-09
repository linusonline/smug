## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugGraphicsTest)

# Header files for Smug library.
include_directories ("${PROJECT_SOURCE_DIR}/../../headers")
# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/../headers")

add_library(SmugGraphicsTest imagetest.c renderbatchtest.c texturetest.c spritesheettest.c)

find_package(OpenGL)
target_link_libraries (SmugGraphicsTest SmugGraphics SmugPlatform glfw ${OPENGL_LIBRARIES})
