#!/bin/sh
gcc `pkg-config --cflags --libs glib-2.0` test.c
