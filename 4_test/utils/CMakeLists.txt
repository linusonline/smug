## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugUtilsTest)

# Add a target to build the library "SmugUtilsTest" from the specified sources.
add_library(SmugUtilsTest linkedlisttest.c binarytreetest.c maptest.c logtest.c stringconsoletest.c)

# Library "SmugUtilsTest" depends on "SmugUtils".
target_link_libraries (SmugUtilsTest SmugUtils)
