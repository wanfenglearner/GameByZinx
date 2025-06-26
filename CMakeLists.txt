#cmake构建版本
cmake_minimum_required(VERSION 3.0)

#项目名称
project(game_zinx)

#设置C++标准
set(CMAKE_CXX_STANDARD 14)

#设置构建模式
set(CMAKE_BUILD_TYPE Debug)

#设置头文件搜索路径
include_directories(${CMAKE_SOURCE_DIR}/include)

#设置源文件搜索
aux_source_directory(${CMAKE_SOURCE_DIR}/src SRC)

#设置库文件搜索路径
link_directories(/usr/lib /usr/local/lib)

#设置可执行文件生成目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)

#生成可执行文件
add_executable(Game_Zinx ${SRC})

#链接动态库
target_link_libraries(Game_Zinx zinx protobuf pthread)



