set(CMAKE_AUTOMOC ON)

add_subdirectory(camera)
add_subdirectory(communication)
add_library(halLib )

target_link_libraries(halLib
    baslerCameraLib
    communicationLib)