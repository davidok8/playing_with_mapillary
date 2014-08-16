cmake_minimum_required(VERSION 2.8.7)

project(DO-CV)
set(DO_VERSION 1.0.0)

# Configure DO++: put the versions.
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DOVersion.cmake.in
               ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DOVersion.cmake @ONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/DO/Defines.hpp.in
               ${CMAKE_CURRENT_SOURCE_DIR}/src/DO/Defines.hpp @ONLY)

# Set default build type to Release mode.
set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Build type options are: Debug, Release")

# Set options.
option(DO_USE_FROM_SOURCE
       "Use DO++ libraries from source"             ON)
option(DO_USE_STATIC_LIBS
       "Use DO++ static libraries"                  OFF)
option(DO_BUILD_SHARED_LIBS
       "Build shared libraries for DO++ libraries"  OFF)
option(DO_BUILD_IMAGE_IO
       "Build third-party image I/O libraries"      ON)

# Add DO-CV to the CMake module path.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(DO REQUIRED)
do_dissect_version()
do_get_os_info()

# Group projects by category.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set installation settings.
include(DOInstall)

# Build third-party software (google test, FLANN,...)
add_subdirectory(third-party)

# Make documentation
add_subdirectory(doc)

# Make packages
include(DOMakePackages)
