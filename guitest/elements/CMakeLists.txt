## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (Elements)

# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}")
include_directories ("${PROJECT_SOURCE_DIR}/../../headers")

add_executable(Elements elements.c map1.c)

find_package(OpenGL)
target_link_libraries(Elements SmugGraphics SmugPlatform SmugUtils SmugInput glfw ${OPENGL_LIBRARIES})
