#!/bin/sh


square()
{
	N=$1
	# if returning a number, it must be caught with $? as exit status
	# of the function
	return `expr $N \* $N`
}

square $1
echo $?

square_string()
{
	N=$1
	# if returning a string, it can be caught by calling a function like
	# $(function arguments)
	echo `expr $N \* $N`
}

echo $(square_string $1)

ans=0

square_ans()
{
	N=$1
	# store result in a shared variable
	ans=`expr $N \* $N`
}

square_ans $1
echo $ans
