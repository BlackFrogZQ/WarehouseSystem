add_subdirectory(stateLedWidget)
add_subdirectory(setParaWindow)

aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(uiLib ${DIR_SYSTEM_SRCS})

target_link_libraries(uiLib
    stateLedWidgetLib
    setParaWindowLib)