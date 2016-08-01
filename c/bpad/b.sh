#!/bin/sh
gcc -g -shared -fPIC -o libcrash.so libcrash.c -g ./src/client/linux/libbreakpad_client.a
g++ -g -o cppcrash crash.cpp breakpad.cpp -g ./src/client/linux/libbreakpad_client.a libcrash.so -I./src -std=c++11 -lpthread
