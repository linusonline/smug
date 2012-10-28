## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugGuiTest)

# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/../headers")

add_subdirectory (elements)
add_subdirectory (audio)

# Add a target to build the executable "SmugGuiTest" from the specified sources.
add_executable(WindowTest window.c)
add_executable(FullscreenTest fullscreen.c)
add_executable(CoordinateTest window-ortho.c)
add_executable(DrawTest drawrect.c)
add_executable(ConsoleDemo console_demo.c)
add_executable(DrawTexture drawtexture.c)
add_executable(SpriteSheet spritesheet.c)
add_executable(Sprite sprite.c)
add_executable(RenderQueue renderqueue.c)
add_executable(Game game.c)

find_package(OpenGL)
target_link_libraries(WindowTest glfw ${OPENGL_LIBRARIES})
target_link_libraries(FullscreenTest glfw ${OPENGL_LIBRARIES})
target_link_libraries(CoordinateTest SmugGraphics glfw ${OPENGL_LIBRARIES})
target_link_libraries(DrawTest SmugGraphics glfw ${OPENGL_LIBRARIES})
target_link_libraries(ConsoleDemo SmugUtils)
target_link_libraries(DrawTexture SmugGraphics SmugPlatform SmugUtils glfw ${OPENGL_LIBRARIES})
target_link_libraries(SpriteSheet SmugGraphics SmugPlatform SmugUtils glfw ${OPENGL_LIBRARIES})
target_link_libraries(Sprite SmugGraphics SmugPlatform SmugUtils glfw ${OPENGL_LIBRARIES})
target_link_libraries(RenderQueue SmugGraphics SmugPlatform SmugUtils glfw ${OPENGL_LIBRARIES})
target_link_libraries(Game SmugGraphics SmugPlatform SmugUtils SmugInput SmugEngine glfw ${OPENGL_LIBRARIES})
