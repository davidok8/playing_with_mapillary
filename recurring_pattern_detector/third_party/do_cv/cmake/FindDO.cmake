# Load DO-specific macros
include(DOMacros)

# Specify DO-CV version.
include(DOVersion)

################################################################################
# Debug message.
do_step_message("FindDO running for project '${PROJECT_NAME}'")

################################################################################
# Setup DO++ once for all for every test projects in the 'test' directory.
if (NOT DO_FOUND)
  if (DEFINED ENV{DO_DIR} AND EXISTS "$ENV{DO_DIR}/cmake/FindDO.cmake") # Check if DO++ is installed
    string(REPLACE "\\" "/" DO_DIR "$ENV{DO_DIR}")
    set(DO_INCLUDE_DIR ${DO_DIR}/include)
  elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindDO.cmake") # DO++ needs to be built or used from source
    message(STATUS "Building DO++ from source")
    set(DO_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(DO_INCLUDE_DIR ${DO_DIR}/src)
  elseif (DO_DIR AND EXISTS "${DO_DIR}/cmake/FindDO.cmake")
    set(DO_INCLUDE_DIR ${DO_DIR}/src)
    do_dissect_version()
    do_get_os_info()
  else ()
    message(FATAL_ERROR "DO++ is not found!")
  endif ()
  
  # DEBUG
  do_step_message("Found DO libraries in directory:")
  message(STATUS "  - DO_DIR = '${DO_DIR}'")

  # Set third-party software directories
  set(DO_SOURCE_DIR ${DO_DIR}/src/DO)
  set(DO_ThirdParty_DIR ${DO_DIR}/third-party)
  set(Eigen3_DIR ${DO_ThirdParty_DIR}/eigen)
  set(jpeg_DIR ${DO_ThirdParty_DIR}/libjpeg)
  set(png_DIR ${DO_ThirdParty_DIR}/libpng)
  set(tiff_DIR ${DO_ThirdParty_DIR}/libtiff)
  
  # List the available component libraries in DO++
  # Foundational libraries
  do_append_components(DO_COMPONENTS Core)
  do_append_components(DO_COMPONENTS ImageIO)
  do_append_components(DO_COMPONENTS ImageProcessing)
  
  # DEBUG: Print the list of component libraries
  do_step_message("Currently available component libraries:")
  foreach (component ${DO_COMPONENTS})
    message (STATUS "  - ${component}")
  endforeach (component)
  
  # Configure compiler for the specific project.
  include (DOConfigureCompiler)

  # Set DO
  set(DO_FOUND TRUE)
  set(DO_USE_FILE UseDO)
endif ()




################################################################################
# Check that the requested libraries exists whenever:
# 'find_package(DO COMPONENTS Core Graphics ... REQUIRED)' is called.
if (DO_FIND_COMPONENTS)
  # Configure compiler for the specific project.
  include (DOConfigureCompiler)
  
  # DEBUG
  do_step_message("Requested libraries by project '${PROJECT_NAME}':")
  foreach (component ${DO_FIND_COMPONENTS})
   do_substep_message ("- ${component}")
  endforeach (component)

  set(DO_USE_COMPONENTS "")
  foreach (component ${DO_FIND_COMPONENTS})
    list(FIND DO_COMPONENTS ${component} COMPONENT_INDEX)
    if (COMPONENT_INDEX EQUAL -1)
      message (FATAL_ERROR "[DO] ${component} does not exist!")
    else ()
      set(DO_${component}_FOUND TRUE)
      list (APPEND DO_USE_COMPONENTS ${component})
    endif ()
  endforeach (component)
endif (DO_FIND_COMPONENTS)
