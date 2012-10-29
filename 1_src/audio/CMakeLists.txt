## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugAudio)

add_library(SmugAudio audio.c)
target_link_libraries (SmugAudio SmugUtils ${MY_LIB_DIR}/libfmodexL.a)
