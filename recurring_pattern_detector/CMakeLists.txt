cmake_minimum_required(VERSION 2.8.7)

project(mapillary_assignment)

if (UNIX)
  # Base compilation flags.
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  # Additional flags for Release builds.
  set(CMAKE_CXX_RELEASE_FLAGS "-03 -ffast-math")
endif ()

include_directories(third_party/graph_cut_optimization)
include_directories(third_party/do_cv/src)
include_directories(third_party/do_cv/third-party/eigen)
include_directories(third_party/do_cv/third-party)
include_directories(third_party/do_cv/third-party/libjpeg)
include_directories(third_party/do_cv/third-party/libpng)
include_directories(third_party/do_cv/third-party/libtiff)
include_directories(third_party/do_cv/third-party/zlib)

add_subdirectory(third_party)
add_subdirectory(recurring_pattern_detector)
