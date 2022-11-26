cmake_minimum_required(VERSION 2.8.3)
project(UnitreeMotorA1B1)

include_directories(
    /home/$ENV{USER}/unitree_actuator_sdk/include/
)

link_directories(
    lib/
)

add_executable(motorctrl example/main.cpp)
target_link_libraries(motorctrl libunitreeMotorSDK_Linux64.so)