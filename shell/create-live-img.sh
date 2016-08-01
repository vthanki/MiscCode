#!/bin/sh

if [ $# -ne 2 ];
then
	echo "Usage:	$0 <target-disk> <image-file>"
	exit
fi


TGTDEV=$1
IMG=$2

echo "Creating partitions ..."

# to create the partitions programatically (rather than manually)
# we're going to simulate the manual input to fdisk
# The sed script strips off all the comments so that we can 
# document what we're doing in-line with the actual commands
# Note that a blank line (commented as "defualt" will send a empty
# line terminated with a newline to take the fdisk default.

# Source: http://superuser.com/questions/332252/creating-and-formating-a-partition-using-a-bash-script

sed -e 's/\s*\([\+0-9a-zA-Z]*\).*/\1/' << EOF | fdisk ${TGTDEV}
  o # clear the in memory partition table
  n # new partition
  p # primary partition
  1 # partition number 1
    # default - start at beginning of disk 
  +100M # 100 MB boot parttion
  n # new partition
  p # primary partition
  2 # partion number 2
    # default, start immediately after preceding partition
    # default, extend partition to end of disk
  a # make a partition bootable
  1 # bootable partition is partition 1 -- /dev/sda1
  p # print the in-memory partition table
  w # write the partition table
  q # and we're done
EOF

sync

mkfs.vfat ${TGTDEV}1
sync
mkfs.ext2 -F ${TGTDEV}2
sync

BOOTIMG_DIR=`mktemp -d -p /tmp`
EXIMT_DIR=`mktemp -d -p /tmp`
ROOTFS_DIR=`mktemp -d -p /tmp`
DTS_DIR=`mktemp -d -p /tmp`

mount ${TGTDEV}1 $BOOTIMG_DIR
cp $IMG $BOOTIMG_DIR/.
touch $BOOTIMG_DIR/live.img
sync

echo "Image copied to USB stick ..."

OFFSET="8658944"
losetup -o $OFFSET /dev/loop0 $IMG
mount -t ext2 -o ro /dev/loop0 $EXIMT_DIR

echo "Image extracted ..."

mount ${TGTDEV}2 $ROOTFS_DIR
tar -C $ROOTFS_DIR -xzf $EXIMT_DIR/rootfs.tgz
mkdir -p $ROOTFS_DIR/dtb-blobs

echo "RootFS copied to USB stick ..."

mount $EXIMT_DIR/dts.cramfs $DTS_DIR
cp $DTS_DIR/* $ROOTFS_DIR/dtb-blobs/.

echo "DTS blobs copied to USB stick ..."

sync

echo "Unmounting the USB stick ..."
umount $ROOTFS_DIR
umount $DTS_DIR
umount $EXIMT_DIR
losetup -D /dev/loop0

echo "Done ..."
