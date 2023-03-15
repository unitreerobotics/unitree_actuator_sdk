cmake_minimum_required(VERSION 2.8.2)
project(UnitreeMotorSDK_M80106)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

include_directories(include)
link_directories(lib)

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64") 
    set(EXTRA_LIBS libUnitreeMotorSDK_M80106_Arm64.so)
else()
    set(EXTRA_LIBS libUnitreeMotorSDK_M80106_Linux64.so)
endif()

add_executable(motorctrl example/main.cpp)
target_link_libraries(motorctrl ${EXTRA_LIBS})
