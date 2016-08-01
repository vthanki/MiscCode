cmake_minimum_required(VERSION 3.0)

project(keycode)

if(TARGET)
	message("Using ARM compiler")
	set(CMAKE_C_COMPILER "arm-linux-gcc")
else()
	message("Using default compiler")
	set(CMAKE_C_COMPILER "gcc")
endif(${TARGET} MATCHES "arm")

set(SOURCE_FILES serial.c)

add_executable(serial ${SOURCE_FILES})
