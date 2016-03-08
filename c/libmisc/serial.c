#include "serial.h"

int lm_set_tty_attr(int fd, int speed, int parity)
{
	struct termios tty;

	if (!isatty(fd)) {
		printf("Not a TTY file descriptor\n");
		return -EBADF;
	}

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
