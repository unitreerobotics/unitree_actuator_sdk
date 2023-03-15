cmake_minimum_required(VERSION 2.8.2)
project(UnitreeMotorA1B1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

include_directories(include)
link_directories(lib)

set(BITNESS 32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(BITNESS 64)
endif()

if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "aarch64") 
    IF(BITNESS EQUAL 64)
        set(EXTRA_LIBS libunitreeMotorSDK_Arm64.so)
    ELSEIF(BITNESS EQUAL 32)
        set(EXTRA_LIBS libunitreeMotorSDK_Arm32.so)
    ENDIF()
else()
    set(EXTRA_LIBS libunitreeMotorSDK_Linux64.so)
endif()

add_executable(motorctrl example/main.cpp)
target_link_libraries(motorctrl ${EXTRA_LIBS})