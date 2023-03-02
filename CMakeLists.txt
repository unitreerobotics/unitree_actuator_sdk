cmake_minimum_required(VERSION 2.8.2)
project(UnitreeMotorSDK_M80106)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

include_directories(include)
link_directories(lib)
add_executable(motorctrl example/main.cpp)
target_link_libraries(motorctrl libUnitreeMotorSDK_M80106_Linux64.so)
