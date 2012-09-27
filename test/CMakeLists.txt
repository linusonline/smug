## Slave config file. Not meant to be used on its own when generating makefiles
## with CMake. Use root folder's config file instead.

cmake_minimum_required (VERSION 2.6)
project (SmugTest)

# Header files for Linus library.
include_directories ("${PROJECT_SOURCE_DIR}/../headers")
# Local header files for tests.
include_directories ("${PROJECT_SOURCE_DIR}/headers")

# Add a target to build the executable "LinusTest" from the specified sources.
add_executable(SmugTest linkedlisttest.c)

# Use library "Linus" when linking together "LinusTest".
target_link_libraries (SmugTest SmugUtils)
