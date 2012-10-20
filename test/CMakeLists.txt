## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugTest)

# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/headers")
include_directories ("${PROJECT_SOURCE_DIR}/../headers")

# Add individual test modules
add_subdirectory (utils)
add_subdirectory (graphics)
add_subdirectory (engine)

# Add a target to build the executable "SmugTest" from the specified sources.
add_executable(SmugTest AllTests.c CuTest.c)

# Library "SmugTest" will depend on "Smug" (or "Smug*") as well as "Smug*Test" libraries.
target_link_libraries (SmugTest SmugUtils SmugUtilsTest SmugGraphics SmugGraphicsTest SmugEngineTest)
