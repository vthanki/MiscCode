#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

/*
 * Original source location:
 * http://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
 */

int set_interface_attribs(int fd, int speed, int parity)
{
	struct termios tty;

	memset (&tty, 0, sizeof tty);

	if (tcgetattr (fd, &tty) != 0) {
		perror("tcgetattr");
		return -1;
	}

	if (cfsetospeed (&tty, speed)) {
		perror("cfsetospeed");
		return -1;
	}
	if (cfsetispeed (&tty, speed)) {
		perror("cfsetispeed");
		return -1;
	}

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
	tty.c_iflag &= ~IGNBRK;
	tty.c_lflag = 0;
	tty.c_oflag = 0;
	tty.c_cc[VMIN]  = 0;
	tty.c_cc[VTIME] = 5;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_cflag |= (CLOCAL | CREAD);
	tty.c_cflag &= ~(PARENB | PARODD);
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty)) {
		perror("tcsetattr");
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int fd;

	if (argc < 2) {
		printf("Invalid arguments. Please provide TTY node\n");
		return -1;
	}

	fd = open (argv[1], O_RDWR | O_NOCTTY | O_SYNC);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	if (set_interface_attribs (fd, B115200, 0))
		printf("Failed to set TTY attributes\n");

	close (fd);
	return 0;
}
