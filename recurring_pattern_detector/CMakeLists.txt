cmake_minimum_required(VERSION 2.8.7)

project(mapillary_assignment)

set(DO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/do_cv)
list(APPEND CMAKE_MODULE_PATH ${DO_DIR}/cmake)

include_directories(third_party/graph_cut_optimization)

add_subdirectory(third_party)
add_subdirectory(recurring_pattern_detector)