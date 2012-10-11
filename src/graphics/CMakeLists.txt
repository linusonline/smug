## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugGraphics)

# Directory for library header files.
include_directories ("${PROJECT_SOURCE_DIR}/../../headers")

add_library(SmugGraphics renderbatch.c renderqueue.c image.c texture.c sprite.c spritesheet.c spriteanimation.c drawable.c camera.c graphics.c lodepng/lodepng.c)
target_link_libraries(SmugGraphics SmugUtils)
