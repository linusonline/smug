## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugGraphics)

add_library(SmugGraphics renderbatch.c renderqueue.c image.c texture.c sprite.c spritesheet.c spriteanimation.c drawable.c camera.c graphics.c lodepng/lodepng.c)
find_package(OpenGL)
target_link_libraries(SmugGraphics SmugUtils SmugPlatform glfw ${OPENGL_LIBRARIES})
