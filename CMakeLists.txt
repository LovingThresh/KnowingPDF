cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

#防止与源码在同一目录构建
if(PROJECT_BINARY_DIR STREQUAL PROJECT_SOURCE_DIR)
    message(FATAL_ERROR "The binary directory cannot be the same as source directory")
endif()

#如果没传入就按Release处理
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

#检查编译器支持cpp版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#添加解决方案
project(KnowingPDF_${CMAKE_BUILD_TYPE} VERSION 1.0 LANGUAGES CXX)

#===========================================#
#跨平台预留
if(WIN32)
    message(STATUS ">> build_case = Windows")
elseif(APPLE)
    message(STATUS ">> build_case = Apple")
elseif(UNIX)
    message(STATUS ">> build_case = Unix")
else()
    message(FATAL_ERROR ">> build_case = Unknown")
endif()

#获取platform和config
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PROJECT_PLATFORM "win32")
else()
    set(PROJECT_PLATFORM "x64")
endif()
set(PROJECT_CONFIG ${CMAKE_BUILD_TYPE})

#保持物理目录到解决方案中
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#增加搜索目录
list(APPEND CMAKE_PREFIX_PATH ${QTDIR})

#总输出目录
set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/Build/${PROJECT_PLATFORM}_${PROJECT_CONFIG}")

#设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIR}/lib")

#启用多核编译
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

#输出一些信息
message(STATUS ">> QT_DIR = ${QTDIR}")
message(STATUS ">> OUTPUT_DIR = ${OUTPUT_DIR}")
message(STATUS ">> CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message(STATUS ">> CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS ">> CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message(STATUS ">> CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}(${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS ">> CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
message(STATUS ">> CMAKE_CXX_FLAGS = " ${CMAKE_CXX_FLAGS})
message(STATUS ">> CMAKE_CXX_FLAGS_DEBUG = " ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS ">> CMAKE_CXX_FLAGS_RELEASE = " ${CMAKE_CXX_FLAGS_RELEASE})
message(STATUS ">> CMAKE_LINKER = ${CMAKE_LINKER}")
message(STATUS ">> CMAKE_EXE_LINKER_FLAGS = ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS ">> CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")
message(STATUS ">> CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS ">> CMAKE_ARCHIVE_OUTPUT_DIRECTORY = ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS ">> CMAKE_LIBRARY_OUTPUT_DIRECTORY = ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

#===========================================#
#设置输出名称
set(TARGET_NAME KnowingPDF)

#扫描.h .hpp和.cpp .cc
set(${TARGET_NAME}_SRC_DIR "${PROJECT_SOURCE_DIR}/Source")
file(GLOB_RECURSE ${TARGET_NAME}_HEADER_IN_SRC LIST_DIRECTORIES false "${${TARGET_NAME}_SRC_DIR}/*.h")
file(GLOB_RECURSE ${TARGET_NAME}_SOURCE LIST_DIRECTORIES false "${${TARGET_NAME}_SRC_DIR}/*.cpp")
source_group(TREE "${${TARGET_NAME}_SRC_DIR}" PREFIX "${TARGET_NAME}\\Headers" FILES ${${TARGET_NAME}_HEADER_IN_SRC})
source_group(TREE "${${TARGET_NAME}_SRC_DIR}" PREFIX "${TARGET_NAME}\\Sources" FILES ${${TARGET_NAME}_SOURCE})

# Add styles.qss to the project
set(${TARGET_NAME}_RESOURCE_DIR "${PROJECT_SOURCE_DIR}/Resource")
set(${TARGET_NAME}_QSS_FILE "${${TARGET_NAME}_RESOURCE_DIR}/styles.qss")
source_group("${TARGET_NAME}\\Resources" FILES ${${TARGET_NAME}_QSS_FILE})

##创建/更新ts文件并提供qm生成动作
find_package(Qt5LinguistTools)
qt5_create_translation(${TARGET_NAME}_QM
    ${${TARGET_NAME}_HEADER_IN_SRC} ${${TARGET_NAME}_SOURCE}
    ${YTPLATFORM_HEADER_IN_SDK} ${YTPLATFORM_HEADER_IN_INC} ${YTPLATFORM_HEADER_IN_SRC} ${YTPLATFORM_SOURCE}
    "${OUTPUT_DIR}/project/${TARGET_NAME}_zh_CN.ts")

##配置依赖
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# 设置PDFium所在的目录
set(PDFium_DIR "${CMAKE_SOURCE_DIR}/Dependencies/pdfium-v8-win-x64")

# 查找 PDFium 包
find_package(PDFium REQUIRED)

#添加项目
add_executable(${TARGET_NAME} WIN32
    ${${TARGET_NAME}_HEADER_IN_SRC} ${${TARGET_NAME}_SOURCE}
    ${${TARGET_NAME}_QM} ${PROJECT_SOURCE_DIR}/Resource/KnowingPDF.qrc)

#头文件搜索路径
target_include_directories(${TARGET_NAME} PRIVATE
    "${${TARGET_NAME}_SRC_DIR}"
)
target_include_directories(${TARGET_NAME} INTERFACE
    "${${TARGET_NAME}_SDK_DIR}"
    "${${TARGET_NAME}_INC_DIR}"
)

# 添加 PDFium 头文件搜索路径
# PDFium_INCLUDE_DIRS 是 PDFiumConfig.cmake 中定义的头文件目录变量
target_include_directories(${TARGET_NAME} PRIVATE ${PDFium_INCLUDE_DIRS})

#将特定警告忽略或视为错误
if(MSVC)
    set_target_properties(${TARGET_NAME} PROPERTIES COMPILE_FLAGS "/we4715 /wd4996")
endif()

#库搜索路径
target_link_libraries(${TARGET_NAME}
    PRIVATE pdfium Qt5::Core Qt5::Gui Qt5::Widgets)

#链接选项
if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS /NODEFAULTLIB:"LIBCMT")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

##打开qt特性配置
set_target_properties(${TARGET_NAME} PROPERTIES AUTOMOC ON)
set_target_properties(${TARGET_NAME} PROPERTIES AUTORCC ON)
set_target_properties(${TARGET_NAME} PROPERTIES AUTOUIC ON)

#设置默认启动项
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT KnowingPDF)

#拷贝资源文件
file(COPY "${PROJECT_SOURCE_DIR}/Resource" DESTINATION "${OUTPUT_DIR}/bin")

#给release拷贝运行依赖库以便构造版本
if(${PROJECT_CONFIG} STREQUAL Release)
    file(COPY "${QTDIR}/bin/Qt5Core.dll" DESTINATION "${OUTPUT_DIR}/bin")
    file(COPY "${QTDIR}/bin/Qt5Gui.dll" DESTINATION "${OUTPUT_DIR}/bin")
    file(COPY "${QTDIR}/bin/Qt5Widgets.dll" DESTINATION "${OUTPUT_DIR}/bin")
    file(COPY "${QTDIR}/plugins/platforms/qwindows.dll" DESTINATION "${OUTPUT_DIR}/bin/plugins/platforms")
endif()

#附录：
#源文件
#target_sources(<target> PRIVATE <source-file>...)
#头文件搜索路径
#target_include_directories(${TARGET_NAME} PRIVATE "${TARGET_NAME}_INC_DIR" "DRAWING_INC_DIR" "")
#预处理的宏定义
#target_compile_definitions(${TARGET_NAME} PRIVATE <macro-definitions>...)
#编译选项
#target_compile_options(${TARGET_NAME} PRIVATE <compile-option>...)
#链接相关的库
#target_link_libraries(${TARGET_NAME} PRIVATE <dependency>...)
#库搜索路径
#target_link_directories(${TARGET_NAME} PRIVATE <linker-search-dir>...)
#链接选项
#target_link_options(${TARGET_NAME} PRIVATE <linker-option>...)
#其它编译特点，例如指定C++标准
#target_compile_features(${TARGET_NAME} PRIVATE <feature>...)