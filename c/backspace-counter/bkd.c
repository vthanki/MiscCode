#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>

static pthread_t handle;
static int fd;
static int counter;
static int usecnt;

#ifdef _BKD_DEBUG
static const char *const evval[3] = {
	"RELEASED",
	"PRESSED ",
	"REPEATED"
};
#endif

static void* backspace_detector(void *arg)
{
	struct input_event ev;
	ssize_t n;

#ifdef _BKD_DEBUG
	fprintf(stderr, "[D] BKD Daemon started ...\n");
#endif
	while (1) {
		n = read(fd, &ev, sizeof ev);
		if (n == (ssize_t)-1) {
			if (errno == EINTR)
				continue;
			else
				break;
		} else if (n != sizeof ev) {
				errno = EIO;
				break;
		}

#ifdef _BKD_DEBUG
		fprintf(stderr, "[D] event type: 0x%x key_code: 0x%x, event_value:%s\n",
				ev.type, ev.code, evval[ev.value]);
#endif

		if (ev.type == EV_KEY && ev.value >= 1 && ev.value <= 2 && ev.code == KEY_BACKSPACE) {
			counter++;
			if (counter == 0)
				fprintf(stderr, "[-] Counter overflowed\n");
		}
	}
}

int bkd_start_daemon(const char* dev)
{
	fd = open(dev, O_RDONLY);
	if (fd == -1) {
		fprintf(stderr, "[-] Cannot open %s: %s.\n", dev, strerror(errno));
		return -1;
	}
	return pthread_create(&handle, NULL, backspace_detector, (void *)dev);
}

void bkd_stop_daemon()
{
	pthread_kill(handle, SIGKILL);
#ifdef _BKD_DEBUG
	fprintf(stderr, "[D] BKD Daemon killed ...\n");
#endif
}

int bkd_get_current_count()
{
	return counter;
}
