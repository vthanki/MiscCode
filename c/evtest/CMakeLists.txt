cmake_minimum_required(VERSION 3.0)

project(evtest)

if(ARM)
	message("Using ARM compiler")
	set(CMAKE_C_COMPILER "/home/vishal.thanki/projects/raumfeld-firmare-workspace/build/com.raumfeld.firmware.buildroot/buildroot-audioadapter-armada/host/usr/bin/arm-linux-gcc")
else()
	message("Using default compiler")
	set(CMAKE_C_COMPILER "gcc")
endif()
set(SOURCE_FILES evtest.c)

add_executable(evtest evtest)
