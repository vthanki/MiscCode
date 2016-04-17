#!/bin/sh

# Reverse iterate from 10 to 1
# Decrement using expr

i=10
while [ $i -gt 0 ];
do
	echo $i
	i=`expr $i - 1`
done
