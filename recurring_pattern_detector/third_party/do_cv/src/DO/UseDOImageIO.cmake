file(GLOB DO_ImageIO_HEADER_FILES ImageIO/*.hpp)
file(GLOB DO_ImageIO_SOURCE_FILES ImageIO/*.cpp)

add_library(DO_ImageIO
            ImageIO.hpp
            ${DO_ImageIO_HEADER_FILES}
            ${DO_ImageIO_SOURCE_FILES})
target_link_libraries(DO_ImageIO jpeg png tiff easyexif)
