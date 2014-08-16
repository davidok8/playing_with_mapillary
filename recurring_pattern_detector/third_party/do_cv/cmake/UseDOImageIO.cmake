macro (do_set_imageio_source_dir)
  set(DO_ImageIO_SOURCE_DIR ${DO_SOURCE_DIR}/ImageIO)
endmacro (do_set_imageio_source_dir)

macro (do_list_imageio_source_files)
  # Master header file
  set(DO_ImageIO_MASTER_HEADER ${DO_SOURCE_DIR}/ImageIO.hpp)
  source_group("Master Header File" FILES ${DO_ImageIO_MASTER_HEADER})
  # Header files
  file(GLOB DO_ImageIO_HEADER_FILES
       ${DO_ImageIO_SOURCE_DIR}/*.hpp)
  # Source files
  file(GLOB DO_ImageIO_SOURCE_FILES
       ${DO_ImageIO_SOURCE_DIR}/*.cpp)
  # All header files here
  set(DO_ImageIO_HEADER_FILES
      ${DO_ImageIO_MASTER_HEADER}
      ${DO_ImageIO_HEADER_FILES})
endmacro (do_list_imageio_source_files)

macro (do_load_packages_for_imageio_library)
  include(${DO_Core_USE_FILE})
  include_directories(
    ${DO_ThirdParty_DIR}
    ${antigrain_DIR}/include
    ${jpeg_DIR}
    ${png_DIR}
    ${tiff_DIR})
endmacro (do_load_packages_for_imageio_library)

macro (do_create_variables_for_imageio_library)
  set(DO_ImageIO_LIBRARIES DO_ImageIO)
  set(DO_ImageIO_LINK_LIBRARIES "jpeg;png;tiff;easyexif")
endmacro (do_create_variables_for_imageio_library)

do_load_packages_for_imageio_library()

if (DO_USE_FROM_SOURCE)
  get_property(DO_ImageIO_ADDED GLOBAL PROPERTY _DO_ImageIO_INCLUDED)
  do_create_variables_for_imageio_library()
  if (NOT DO_ImageIO_ADDED)
    do_set_imageio_source_dir()
    do_list_imageio_source_files()
    do_generate_library("ImageIO")
  endif ()
endif ()
