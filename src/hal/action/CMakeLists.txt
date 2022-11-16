add_subdirectory(reset)
add_subdirectory(autoWork)
aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(actionLib ${DIR_SYSTEM_SRCS})
target_link_libraries(actionLib
    resetActionLib
    autoWorkActionLib)