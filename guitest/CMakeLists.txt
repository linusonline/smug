## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugGuiTest)

# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/../headers")

# Add a target to build the executable "SmugGuiTest" from the specified sources.
add_executable(SmugGuiTest guitest.c)

# Program "SmugGuiTest" will depend on "SmugUtils".
target_link_libraries (SmugGuiTest SmugUtils)
target_link_libraries (SmugGuiTest glfw)

find_package(OpenGL)
target_link_libraries(SmugGuiTest ${OPENGL_LIBRARIES})
