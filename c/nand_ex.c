/*
 * The original code was taken from following link.
 * http://stackoverflow.com/questions/15336285/write-on-a-mtd-block-device
 *
 * It is modified for the demonstration purpose.
 */

#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

/*
 * Write data length and write data offset must be aligned to subpage size.
 * The subpage size should be read from sysfs interface for a particular
 * partition. The file should be located at:
 * /sys/class/mtd/mtdX/subpagesize - where X is the MTD partition number.
 */
#define SUBPAGE_SIZE	512


#define MTDPART		"/dev/mtd3"

/*
 * This program does following
 * ============================
 * 1. First of all, read 16 bytes from the beginning of a MTD partition
 * 2. Erase MTD partition to perform a new write on the same location i.e.
 *    beginning of the MTD partition
 * 3. Read the MTD partition to confirm that erase was successful. All the read
 *    data must be 0xFF.
 * 4. Perform write, with unaligned data length - write will fail.
 * 5. Perform write, with unaligned offset - write will fail.
 * 6. Perform write with aligned data length and aligned offset - write will
 *    pass.
 * 7. Perform a new write, but at a different offset to demonstrate that erase
 *    cycle is not required if data is not being overwritten.
 * 8. Validate the write by reading back the data.
 */

int main(int argc, char *argv[])
{
	mtd_info_t mtd_info;
	erase_info_t ei;
	unsigned char read_buf[16]  = {0}, *read_buf_ptr;
	unsigned char write_buf[16], *write_buf_ptr;
	int i, ret, fd;

	fd = open(MTDPART, O_RDWR);
	if (fd < 0) {
		printf("Fail to open MTD device node\n");
		return fd;
	}

	/* 1. First, read 16 bytes from the beginning of a MTD partition */

	printf("Performing read from the beginning of MTD partition ...\n");
	ret = read(fd, read_buf, sizeof(read_buf));
	if (ret < 0) {
		printf("Read failed : %d\n", ret);
		goto exit;
	}

	printf("Data: ");
	for (i = 0; i < sizeof(read_buf); i++)
		printf("0x%02x ", read_buf[i]);
	printf("\n");


       /*
	* 2. Erase MTD partition to perform a new write on the same location
	* i.e. beginning of the MTD partition
	*/

	printf("Erasing MTD partition and reading again ...\n");
	/* The MTD information is required to erase the partition */
	if ((ret = ioctl(fd, MEMGETINFO, &mtd_info)) < 0) {
		printf("Cannot get MTD information\n");
		goto exit;
	}

	/* Erase the partition */
	ei.length = mtd_info.erasesize;
	for(ei.start = 0; ei.start < mtd_info.size; ei.start += ei.length) {
		ret = ioctl(fd, MEMERASE, &ei);
		if (ret < 0) {
			printf("Ioctl failed with error:%d\n", ret);
			goto exit;
		}
	}

       /*
	* 3. Read the MTD partition to confirm that erase was successful.
	*    All the read data must be 0xFF.
	*/

	ret = read(fd, read_buf, sizeof(read_buf));
	if (ret < 0) {
		printf("Read failed : %d\n", ret);
		goto exit;
	}

	printf("Data: ");
	for (i = 0; i < sizeof(read_buf); i++)
		printf("0x%02x ", read_buf[i]);
	printf("\n");


	/* 4. Perform write, with unaligned data length - write will fail. */

	printf("Performing write with unaligned data length ...\n");
	lseek(fd, 0, SEEK_SET);
	memset(write_buf, 0xa5, sizeof(write_buf));

	ret = write(fd, write_buf, sizeof(write_buf));
	if (ret < 0) {
		printf("Write failed, unaligned data length : %d\n", ret);
	}

 	/* 5. Perform write, with unaligned offset - write will fail. */

	printf("Performing write with unaligned partition offset ...\n");
	lseek(fd, 10, SEEK_SET);

	/* Let us make data len aligned */
	write_buf_ptr = (char *)malloc(SUBPAGE_SIZE);
	if (!write_buf_ptr) {
		printf("malloc failed for write_buf\n");
		goto exit;
	}

	memset(write_buf_ptr, 0xa5, SUBPAGE_SIZE);
	ret = write(fd, write_buf_ptr, SUBPAGE_SIZE);
	if (ret < 0) {
		printf("Write failed, unaligned partition offset : %d\n", ret);
	}

 	/*
	 * 6. Perform write with aligned data length and aligned offset -
	 *    write will pass.
	 */

	printf("Performing write with aligned data and aligned offset ...\n");
	lseek(fd, 0, SEEK_SET);

	ret = write(fd, write_buf_ptr, SUBPAGE_SIZE);
	if (ret < 0) {
		printf("Write failed : %d\n", ret);
		goto exit;
	}

       /*
	* 7. Perform a new write, but at a different offset to demonstrate that erase
	*    cycle is not required if data is not being overwritten.
	*/

	printf("Performing write with aligned data and at different (aligned) offset ...\n");
	lseek(fd, 2*SUBPAGE_SIZE, SEEK_SET);
	ret = write(fd, write_buf_ptr, SUBPAGE_SIZE);
	if (ret < 0) {
		printf("Write failed : %d\n", ret);
		goto exit;
	}

       /* 8. Validate the write by reading back the data. */

	printf("Validating the write ...\n");
	lseek(fd, 2*SUBPAGE_SIZE, SEEK_SET);
	read_buf_ptr = (char *)malloc(SUBPAGE_SIZE);
	if (!read_buf_ptr) {
		printf("malloc failed for read_buf\n");
		goto exit;
	}

	memset(read_buf_ptr, 0x0, SUBPAGE_SIZE);

	ret = read(fd, read_buf_ptr, SUBPAGE_SIZE);
	if (ret < 0) {
		printf("read failed : %d\n", ret);
		goto exit;
	}

	ret = 0;
	for (i = 0; i < SUBPAGE_SIZE; i++) {
		if (read_buf_ptr[i] != write_buf_ptr[i]) {
			printf("Data mismatch at %d offset\n", i);
			printf("Expected :0x%02x, Readback :0x%02x\n",
					write_buf_ptr[i], read_buf_ptr[i]);
			ret = -1;
			break;
		}
	}

	if (!ret)
		printf("Write was OK\n");

	free(read_buf_ptr);
	free(write_buf_ptr);
	close(fd);
	return ret;

exit:
	close(fd);
	return ret;
}
