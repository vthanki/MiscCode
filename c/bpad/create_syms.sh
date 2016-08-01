#!/bin/sh

if [ $# -ne 1 ];then
	echo "Usage: $0 <binary>"
	exit
fi

DUMP_SYMS=./src/tools/linux/dump_syms/dump_syms
SYMID=`$DUMP_SYMS $1 | head -n1 | awk '{print $4}'`

if [ $? -ne 0 ];then
	echo "Failed to extract symbol info from $1"
	exit
fi

mkdir -p symbols/$1/$SYMID/
$DUMP_SYMS $1 > symbols/$1/$SYMID/$1.sym

echo "Symbol file created at symbols/$1/$SYMID/$1.sym"
