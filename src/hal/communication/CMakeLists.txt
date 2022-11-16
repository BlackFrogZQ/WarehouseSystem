aux_source_directory(. DIR_SYSTEM_SRCS)
add_library(communicationLib ${DIR_SYSTEM_SRCS})
target_link_libraries(communicationLib Qt5::SerialPort Qt5::SerialBus) # Qt5 Shared Library
