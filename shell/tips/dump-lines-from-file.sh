#!/bin/sh
while read line
do
	echo $line | ./a.out
done < $1
