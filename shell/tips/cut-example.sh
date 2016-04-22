#!/bin/sh

# This example prints CPU flags by extracting it from /proc/cpuinfo. Using
# 'cut', the script obtains the values printed after ":"

flags=`cat /proc/cpuinfo | grep flags | head -n 1 | cut -d":" -f2`

for f in $flags
do
	echo $f
done
