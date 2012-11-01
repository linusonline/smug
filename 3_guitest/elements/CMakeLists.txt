## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (Elements)

include_directories ("${PROJECT_SOURCE_DIR}")

add_executable(Elements elements.c map1.c avatar.c monster.c attack.c actiongauge.c characterlogic.c objects.c monsters/tortoise.c ambience.c)

find_package(OpenGL)
target_link_libraries(Elements SmugGraphics SmugPlatform SmugUtils SmugInput SmugEngine SmugAudio glfw ${OPENGL_LIBRARIES} ${MY_LIB_DIR}/libfmodexL.a)

file(COPY "${MY_LIB_DIR}/fmodexL.dll" DESTINATION "${PROJECT_BINARY_DIR}")
