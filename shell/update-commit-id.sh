#!/bin/sh

echo_usage() {
cat << __EOF__ >&2
Usage: $0
 [ --kernel=<commit_id>  --kernel-path=<kernel-tree-path> ]
 [ --dts=<commit_id>  --dts-path=<dts-tree-path> ]
 [ --mcu=<commit_id>  --mcu-path=<mcu-tree-path> ]
 [ --rf-extract=<commit_id>  --rf-extract-path=<rf-extract-tree-path> ]

Example: $0 --mcu=b91d632179b49ff4c7d893349ff628ae59134e8c --mcu-path=~/projects/git/mcu-protocol
__EOF__
	exit 1
}

. ./getopt.inc
getopt $*

if [ -z "$kernel" ] && \
   [ -z "$mcu" ]  && \
   [ -z "$dts" ]  && \
   [ -z "$rf_extract" ]
then echo_usage; fi

update_commit()
{
	tree_id=$1
	commit_id=$2
	tree_path=$3

	kernel_token="set(LINUX_KERNEL_GIT_REF"
	dts_token="set(RAUMFELD_DTS_GIT_REF"
	mcu_token="set(MCU_PROTOCOL_GIT_REF"
	rf_extract_token="set(RAUMFELD_EXTRACT_GIT_REF"
	filename=CMakeLists.txt

	cd $tree_path
	if [ $? -ne 0 ]; then
		echo "$tree_path is not valid for $tree_id"
		exit 1
	fi

	git --no-pager show $commit_id > /dev/null 2>&1
	if [ $? -ne 0 ]; then
		echo "$commit_id is not valid commit id for $tree_id at $tree_path/"
		echo "Hint: Is $tree_id is valid git repo? OR Missing 'git pull'?"
		exit 1
	fi

	cd -

	case "$tree_id" in
		kernel)
			sed -i "s/${kernel_token} .*$/${kernel_token} \"${commit_id:0:10}\" CACHE STRING/" ${filename}
			git diff
			;;
		dts)
			sed -i "s/${dts_token} .*$/${dts_token} \"${commit_id:0:10}\" CACHE STRING/" ${filename}
			git diff
			;;
		mcu)
			sed -i "s/${mcu_token} .*$/${mcu_token} \"${commit_id:0:10}\" CACHE STRING/" ${filename}
			git diff
			;;
		rf_extract)
			sed -i "s/${rf_extract_token} .*$/${rf_extract_token} \"${commit_id:0:10}\" CACHE STRING/" ${filename}
			git diff
			;;
		*)
			echo "commit-id update for $tree_id not supported in $filename yet"
			exit 1
			;;
	esac
}

if [ ! -z "$kernel" ] && [ ! -z $kernel_path ]; then
	update_commit kernel $kernel $kernel_path
elif [ ! -z "$mcu" ] && [ ! -z $mcu_path ]; then
	update_commit mcu $mcu $mcu_path
elif [ ! -z "$dts" ] && [ ! -z $dts_path ]; then
	update_commit dts $dts $dts_path
elif [ ! -z "$rf_extract" ] && [ ! -z $rf_extract_path ]; then
	update_commit rf_extract $rf_extract $rf_extract_path
else
	echo_usage
fi
