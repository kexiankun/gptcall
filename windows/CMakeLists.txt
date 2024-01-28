cmake_minimum_required(VERSION 3.5)

project(CMake)

# 设置发布版本
#set(CMAKE_BUILD_TYPE Release)

# 添加nuklearGui源代码目录
aux_source_directory(. SRC_LIST)
include_directories(.)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SRC_LIST} main.c)

# 链接对应的函数库
target_link_libraries(${PROJECT_NAME}  CURL user32 gdi32 Msimg32 )




