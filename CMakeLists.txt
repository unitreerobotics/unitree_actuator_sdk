cmake_minimum_required(VERSION 2.8.2)
project(UnitreeMotorA1B1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

include_directories(include)
link_directories(lib)

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64") 
    add_executable(motorctrl example/main.cpp )
    target_link_libraries(motorctrl unitreeMotorSDK_Arm64)

    add_executable(changeID example/changeID.cpp)
    target_link_libraries(changeID unitreeMotorSDK_Arm64)

else()
    add_executable(motorctrl example/main.cpp )
    target_link_libraries(motorctrl unitreeMotorSDK_Linux64)

    add_executable(changeID example/changeID.cpp)
    target_link_libraries(changeID unitreeMotorSDK_Linux64)

endif()
