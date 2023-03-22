cmake_minimum_required(VERSION 3.0.0)
project(main)

set(CMAKE_CXX_STANDARD 17)

project(procotol)

message("⬇⬇⬇ There are some debug info. ⬇⬇⬇")
message("📁项目目录:" ${PROJECT_SOURCE_DIR})
message("📌项目名称:" ${PROJECT_NAME})

message("🔉 cmakeList所在文件夹:" ${CMAKE_CURRENT_SOURCE_DIR})
# remove last end of "/"
string(REGEX REPLACE "/$" "" CURRENT_FOLDER_ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR})

message("    -当前路径:" ${CURRENT_FOLDER_ABSOLUTE})
get_filename_component(CURRENT_FOLDER ${CURRENT_FOLDER_ABSOLUTE} NAME)
message("    -当前目录:" ${CURRENT_FOLDER})

#------------------------------------------------------------------------------
set(SRC_PROTOCOL /protocol)
set(SRC_UTILS /utils)
#------------------------------------------------------------------------------
# Debug版本和Release版本可执行文件的输出目录
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# 可执行文件的输出目录   
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 包含头文件
include_directories (${CURRENT_FOLDER_ABSOLUTE}${SRC_PROTOCOL} ${CURRENT_FOLDER_ABSOLUTE}${SRC_UTILS})
# 源文件目录取名
aux_source_directory(${CURRENT_FOLDER_ABSOLUTE}${SRC_PROTOCOL}  DIR_SRCS_PROTOCOL)
aux_source_directory(${CURRENT_FOLDER_ABSOLUTE}${SRC_UTILS}  DIR_SRCS_UTILS)

LINK_LIBRARIES(m pthread)
#------------------------------------------------------------------------------
# 生成的可执行文件拥有符号表，用于gdb调试
add_definitions("-Wall -g")
# 编译
add_executable(${PROJECT_NAME} main.cpp ${DIR_SRCS_PROTOCOL} ${DIR_SRCS_UTILS})