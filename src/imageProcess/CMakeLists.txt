add_subdirectory(createRoi)
add_subdirectory(discernDirection)
aux_source_directory(. DIR_SYSTEM_SRCS)
set(cppFile imageProcess.h)
add_library(halconProcessLib ${DIR_SYSTEM_SRCS}
                                ${cppFile})

target_link_libraries(halconProcessLib
    createRoiLib
    discernDirectionLib)