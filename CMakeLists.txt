cmake_minimum_required(VERSION 3.0)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)  # 设置 C++ 标准

# 添加可执行文件
add_executable(main main.cpp Berth.cpp Boat.cpp Cargo.cpp Robot.cpp tool.cpp)
add_definitions("-Wall -g -pg")
# 包含头文件路径
include_directories(${CMAKE_SOURCE_DIR})

# 链接库（如果有）

# 安装规则（如果需要）

# 其他配置（根据项目需求添加）