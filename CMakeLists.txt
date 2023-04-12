cmake_minimum_required(VERSION 3.0.0)
project(assemble VERSION 0.1.0)

# 打开我的电脑>属性>高级系统设置>环境变量
# 新建用户变量 变量名"QT5.12.5"，变量值"qt5.12.5下编译器的目录，比如我的是'C:\Qt\Qt5.12.5\5.12.5\msvc2017_64'"
# 同理 新建用户变量 变量名"OrionLib"，变量值"OrionLib所在目录，比如我的是在'C:\OrionLib'"
# 确定保存即可，cmake访问系统变量 $ENV{变量名},如:访问上述变量'QT5.12.5变量' $ENV{QT5.12.5}
# 添加用户变量后需重启
# 自动打包需要在setting.json 添加 "cmake.setBuildTypeOnMultiConfig": true,
SET(CMAKE_PREFIX_PATH $ENV{QT5.14.0})
include_directories(${PROJECT_SOURCE_DIR})
include_directories($ENV{QT5.14.0}\\include)
include_directories($ENV{QT5.14.0}\\include\\QtGui)
include_directories($ENV{QT5.14.0}\\include\\QtWidgets)
include_directories($ENV{QT5.14.0}\\include\\QtCore)
include_directories($ENV{QT5.14.0}\\include\\QtNetwork)
include_directories($ENV{QT5.14.0}\\include\\QtSerialBus)
include_directories($ENV{QT5.14.0}\\include\\QtSerialPort)
include_directories($ENV{QT5.14.0}\\include\\QtConcurrent)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
add_definitions(-DNOMINMAX)
message("Looking for Qt...")
find_package(Qt5 COMPONENTS Core Gui Network SerialBus Widgets SerialPort Concurrent Qml REQUIRED)

if(${Qt5_FOUND})
    message("Found Qt " ${Qt5_VERSION})
else()
    message(FATAL_ERROR "Couldn't find Qt")
endif()

execute_process(
    COMMAND chcp 65001
)

set(Qt5model
    Qt5::Widgets
    Qt5::Network
    Qt5::SerialBus
    Qt5::Gui
    Qt5::Core
    Qt5::SerialPort
    Qt5::Concurrent)

set(isRelease TRUE)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(isRelease FALSE)
    add_definitions(-DTrace)
endif()

include_directories(include/halcon12x64 include/halcon12x64/halconcpp)
include_directories(${PROJECT_SOURCE_DIR}/src/qtSingleApplication)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/qtSingleApplication)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/system)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/para)
set(paraServerCpp
    src/para/define/paraDef.cpp
    src/para/define/communicationDef.cpp
    src/para/define/cameraDef.cpp
    src/para/define/roiRegionYcgDef.cpp
    src/para/define/cameraLzDef.cpp
    src/para/define/roiRegionLzDef.cpp
    src/para/define/serialPortDef.cpp)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/ui)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/hal)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/imageProcess)

file(GLOB libs "lib/halcon12x64/*.lib")

# set(srcCpp
#     src/ui/mainWindow.cpp)

# set(sources
#     ${paraServerCpp}
#     main.cpp
# )

qt5_add_resources(sources qml.qrc)

# if(${isRelease})
#     add_executable(${PROJECT_NAME}
#         WIN32
#         ${sources}
#         assemble.rc)
# else()
#     add_executable(${PROJECT_NAME}
#         ${sources}
#         assemble.rc)
# endif()

# Specify MSVC UTF-8 encoding
# add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
# add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(${PROJECT_NAME}
    WIN32 # If you need a terminal for debug, please comment this statement#是在调试控制台输出还是在终端输出//有顺序要求
    ${paraServerCpp}
    ${sources}
    main.cpp
    ico.rc)

target_link_libraries(${PROJECT_NAME} PRIVATE ${libs} ${Qt5model} qtSingleApplicationLib halLib sysLib halconProcessLib paraLib uiLib) # Qt5 Shared Library

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


if(DEFINED CMAKE_BUILD_TYPE)
    set(exeDir ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    # 所有非Qt dll库 不一定在统一目录 可直接追加目录
    file(GLOB libDlls
        "${PROJECT_SOURCE_DIR}/lib/halcon12x64/*.dll"
        "${PROJECT_SOURCE_DIR}/lib/pylon5x64/*.dll")
    message("${PROJECT_NAME}.exe 所在目录:" ${exeDir})
    message("拷贝 libDlls 库到程序运行目录...")
    string(REPLACE ";" "\n\t" outLibDllNames "${libDlls}")
    message("libDlls:\n\t${outLibDllNames}")
    file(COPY ${libDlls} DESTINATION ${exeDir})
    message("拷贝结束...")
    message("运行qt打包程序...")
    execute_process(
        COMMAND windeployqt ${exeDir}\\${PROJECT_NAME}.exe
        WORKING_DIRECTORY $ENV{QT5.14.0}\\bin
    )
    message("qt打包结束")
else()
    message("未在用户设置中设置 'cmake.setBuildTypeOnMultiConfig'为 true,无法打包")
endif()