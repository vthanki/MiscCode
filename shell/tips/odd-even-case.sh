#!/bin/sh
case $1 in
	2|4|6|8|10)
		echo "Even"
		;;
	1|3|5|7|9)
		echo "Odd"
		;;
	*)
		echo "Greater than 10"
		;;
esac
