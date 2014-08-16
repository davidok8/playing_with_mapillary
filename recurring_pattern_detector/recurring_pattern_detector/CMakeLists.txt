cmake_minimum_required(VERSION 2.8.7)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

add_library(visualization
            visualization.hpp visualization.cpp)

macro (do_add_executable NAME SOURCE)
  add_executable(${NAME} ${SOURCE})
  target_link_libraries(${NAME} DO_Core DO_ImageIO)
  set_target_properties(
    ${NAME} PROPERTIES
    COMPILE_FLAGS "-DSRCDIR=${CMAKE_CURRENT_SOURCE_DIR}"
    COMPILE_DEFINITIONS DO_STATIC
  )
endmacro (do_add_executable)

do_add_executable(recurring_pattern_detector recurring_pattern_detector.cpp)
target_link_libraries(recurring_pattern_detector visualization graph_cut_optimization)
                  
do_add_executable(temporal_statistics temporal_statistics.cpp)
target_link_libraries(temporal_statistics visualization)
