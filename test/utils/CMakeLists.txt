## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugUtilsTest)

# Header files for Smug library.
include_directories ("${PROJECT_SOURCE_DIR}/../../headers")
# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/../headers")

# Add a target to build the library "SmugUtilsTest" from the specified sources.
add_library(SmugUtilsTest linkedlisttest.c)

# Library "SmugUtilsTest" depends on "SmugUtils".
target_link_libraries (SmugUtilsTest SmugUtils)
