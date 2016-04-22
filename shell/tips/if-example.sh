#!/bin/sh

# Check number of arguments

if [ $# -eq 0 ]; then
	echo "There are no arguments passed to script"
else
	echo "There are $# arguments passed to script"
fi

if [ $# -gt 0 ]; then
	if [[ $1 == 100 ]]; then
		echo "Argument is 100"
	elif [[ $1 == "string" ]]; then
		echo "Argument is string"
	else
		echo "Argument is $1"
	fi
fi
