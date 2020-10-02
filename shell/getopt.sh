#!/bin/sh

# Script to take the arguments in following form:
# --arg1-type=value1 --arg2-type=value2
# And convert arguments into shell variables like below:
# arg1_type, arg2_type
# And the variables will be assigned value1 and value2 respectively.

getopt()
{
	#echo "getopt_simple()"
	#echo "Parameters are '$*'"

	until [ -z "$1" ]; do
		if [ ${1:0:2} != "--" ]; then
			echo "Error parsing command line args."
			exit 1
		fi

		tmp=${1#--}

		parameter=${tmp%%=*}
		var=${parameter//-/_}

		value=${tmp##$parameter}
		if [ -z "$value" ]; then
			value=$2

			[ -z "$value" ] && value=1
			[ "${2:0:2}" != "--" ] && shift
		else
			value=${value##=}
		fi

		#echo "Parameter: '$parameter', value: '$value'"
		eval $var=$value

		shift
	done
}
