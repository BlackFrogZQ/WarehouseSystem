# add_subdirectory(order)
aux_source_directory(. DIR_SYSTEM_SRCS)
set(cppFile discernDirection/discernLzDirection.cpp
            discernDirection/discernYcgDirection.cpp)
add_library(discernDirectionLib
    ${DIR_SYSTEM_SRCS}
    ${cppFile})