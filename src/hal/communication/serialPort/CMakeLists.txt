aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(serialPortLib ${DIR_SYSTEM_SRCS})
target_link_libraries(serialPortLib Qt5::SerialPort Qt5::SerialBus) # Qt5 Shared Library
