set(CMAKE_AUTOMOC ON)

add_subdirectory(communication)
add_subdirectory(communication/serialPort)
aux_source_directory(. DIR_SYSTEM_SRCS)

add_library(halLib ${DIR_SYSTEM_SRCS})

target_link_libraries(halLib
    communicationLib
    serialPortLib)