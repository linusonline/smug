## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugEngine)

add_library(SmugEngine engine.c mainloop.c gameobject.c collisiondetector.c body.c)
target_link_libraries (SmugEngine SmugGraphics SmugUtils)
