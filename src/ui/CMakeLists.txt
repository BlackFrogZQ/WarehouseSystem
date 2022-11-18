aux_source_directory(. DIR_SYSTEM_SRCS)
set(cppFile
    inputDialog/inputDialog.cpp
    controlWidget/controlWidget.cpp
    stateLedWidget/plcStateLed.cpp)
add_library(uiLib
    ${DIR_SYSTEM_SRCS}
    ${cppFile})