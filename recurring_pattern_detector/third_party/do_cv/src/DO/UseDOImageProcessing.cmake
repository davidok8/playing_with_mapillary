file(GLOB DO_ImageProcessing_HEADER_FILES
    ${DO_ImageProcessing_SOURCE_DIR}/*.hpp)

list(APPEND DO_ImageProcessing_HEADER_FILES ImageProcessing.hpp)

add_custom_target(DO_ImageProcessing ${DO_ImageProcessing_HEADER_FILES})
