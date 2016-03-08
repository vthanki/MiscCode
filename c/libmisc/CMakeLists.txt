cmake_minimum_required(VERSION 3.0)

project(misc-library)

if(ARM)
	message("Using ARM compiler")
	set(CMAKE_C_COMPILER "arm-linux-gcc")
else()
	message("Using default compiler")
	set(CMAKE_C_COMPILER "gcc")
endif()
set(SOURCE_FILES serial.c uinput.c)

add_library(misc SHARED ${SOURCE_FILES})
