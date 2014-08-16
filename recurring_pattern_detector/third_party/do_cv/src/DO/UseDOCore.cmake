file(GLOB DO_Core_HEADER_FILES FILES Core/*.hpp)
list(APPEND DO_Core_HEADER_FILES ${DO_Core_MASTER_HEADER})
file(GLOB DO_Core_SOURCE_FILES FILES Core/*.cpp)
  
add_library(DO_Core
            Core.hpp
            ${DO_Core_HEADER_FILES}
            ${DO_Core_SOURCE_FILES})
