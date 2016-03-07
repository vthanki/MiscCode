cmake_minimum_required(VERSION 3.0)

project(keycode)

if(ARM)
	message("Using ARM compiler")
	set(CMAKE_C_COMPILER "arm-linux-gcc")
else()
	message("Using default compiler")
	set(CMAKE_C_COMPILER "gcc")
endif()

set(SOURCE_FILES keycode.c)

add_executable(gen-keycode ${SOURCE_FILES})
