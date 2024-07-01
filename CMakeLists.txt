cmake_minimum_required(VERSION 3.1.0)
project(unitree_actuator_sdk)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(include)
link_directories(lib)

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64") 
    set(EXTRA_LIBS libUnitreeMotorSDK_Arm64.so)
else()
    set(EXTRA_LIBS libUnitreeMotorSDK_Linux64.so)
endif()

#example
add_executable(example_a1_motor example/example_a1_motor.cpp)
target_link_libraries(example_a1_motor ${EXTRA_LIBS})

add_executable(example_a1_motor_output example/example_a1_motor_output.cpp)
target_link_libraries(example_a1_motor_output ${EXTRA_LIBS})

add_executable(example_b1_motor example/example_b1_motor.cpp)
target_link_libraries(example_b1_motor ${EXTRA_LIBS})

add_executable(example_goM8010_6_motor example/example_goM8010_6_motor.cpp)
target_link_libraries(example_goM8010_6_motor ${EXTRA_LIBS})

add_executable(changeID example/changeID.cpp)
target_link_libraries(changeID ${EXTRA_LIBS})

set(LIBRARY_OUTPUT_PATH "../lib")
add_subdirectory(thirdparty/pybind11)
pybind11_add_module(unitree_actuator_sdk thirdparty/python_wrapper/wrapper.cpp)
target_link_libraries(unitree_actuator_sdk PRIVATE ${EXTRA_LIBS})
set_target_properties(unitree_actuator_sdk PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_OUTPUT_PATH}")
