set(THIRDPARTY_FOLDER_NAME "DO Third-Party")

message(STATUS "  - zlib")
add_subdirectory(zlib)
set_property(TARGET zlib PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

message(STATUS "  - libjpeg")
add_subdirectory(libjpeg)
set_property(TARGET jpeg PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

message(STATUS "  - libpng")
add_subdirectory(libpng)
set_property(TARGET png PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})

message(STATUS "  - libtiff")
add_subdirectory(libtiff)
set_property(TARGET tiff PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})
  
message(STATUS "  - easyexif")
add_subdirectory(easyexif)
set_property(TARGET easyexif PROPERTY FOLDER ${THIRDPARTY_FOLDER_NAME})
