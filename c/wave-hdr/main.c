#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

struct wavehdr {
	char chunk_id[4];
	int chunk_size;
	char format[4];

	char subchunk1_id[4];
	int subchunk1_size;
	short audio_fmt;
	short nr_channels;
	int sample_rate;
	int byte_rate;
	short block_align;
	short bits_per_sample;

	char subchunk2_id[4];
	int subchunk2_size;
} __attribute__ ((packed));

int main(int argc, char *argv[])
{
	struct stat fbuf;
	int fsize;
	struct wavehdr whdr;
	int fdout;
	char buf[256];
	int read_bytes;
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		printf("open failed :%d\n", fd);
		return fd;
	}

	if (fstat(fd, &fbuf)) {
		printf("fstat failed\n");
		goto exit;
	}

	fsize = fbuf.st_size;
	printf("File size :%d\n", fsize);

	strcpy(whdr.chunk_id, "RIFF");
	strcpy(whdr.format, "WAVE");
	strcpy(whdr.subchunk1_id, "fmt ");
	strcpy(whdr.subchunk2_id, "data");

	whdr.chunk_size = fsize - 8 + sizeof(struct wavehdr);
	whdr.subchunk1_size = 0x10; //Fixed for PCM
	whdr.audio_fmt = 0x1; //PCM
	whdr.nr_channels = 2; //stereo
	whdr.sample_rate = 44100; //CD Quality
	whdr.bits_per_sample = 16;
	whdr.byte_rate = (whdr.sample_rate * whdr.nr_channels * whdr.bits_per_sample) / 8;
	whdr.block_align = whdr.nr_channels * whdr.bits_per_sample / 8;
	whdr.subchunk2_size = fsize;

	fdout = open("/tmp/testwave", O_RDWR | O_CREAT, S_IRUSR | S_IRGRP | S_IROTH);
	if (fdout < 0) {
		printf("create failed\n");
		goto exit;
	}

	write(fdout, &whdr, sizeof(whdr));
	while ((read_bytes = read(fd, buf, sizeof(buf))) > 0) {
		write(fdout, buf, read_bytes);
	}

	close(fdout);
exit:
	close(fd);
	return 0;
}
