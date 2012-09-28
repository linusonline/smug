## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugGuiTest)

# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/../headers")

# Add a target to build the executable "SmugGuiTest" from the specified sources.
add_executable(WindowTest window.c)
add_executable(FullscreenTest fullscreen.c)
add_executable(CoordinateTest window-ortho.c)
add_executable(DrawTest drawrect.c)

find_package(OpenGL)
target_link_libraries(WindowTest glfw ${OPENGL_LIBRARIES})
target_link_libraries(FullscreenTest glfw ${OPENGL_LIBRARIES})
target_link_libraries(CoordinateTest SmugGraphics glfw ${OPENGL_LIBRARIES})
target_link_libraries(DrawTest SmugGraphics glfw ${OPENGL_LIBRARIES})
