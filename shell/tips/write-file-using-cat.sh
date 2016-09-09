#!/bin/sh

VARIABLE=12345
if [ ! -f /tmp/testfile ]; then
	cat << __EOF__ > /tmp/testfile
This
is a multiple line example
containing variable = ${VARIABLE}
__EOF__
fi
