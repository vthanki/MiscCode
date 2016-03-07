#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)

static int keys[] = {
	KEY_PLAYPAUSE,
	KEY_NEXTSONG,
	KEY_PREVIOUSSONG,
	KEY_STOP,
	KEY_FORWARD,
	KEY_REWIND,
	KEY_VOLUMEUP,
	KEY_VOLUMEDOWN,
	KEY_MUTE,
};

static int register_keys(int fd, int *key_arr, int size)
{
	int i;

	if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
		die("error: ioctl");

	for (i = 0; i < size; i++)
		if(ioctl(fd, UI_SET_KEYBIT, key_arr[i]) < 0)
			die("error: ioctl");

	return 0;
}

static int gen_keystroke(int fd, int key)
{
	struct input_event     ev;

	memset(&ev, 0, sizeof(struct input_event));
	ev.type = EV_KEY;
	ev.code = key;
	ev.value = 1;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		die("error: write");

	memset(&ev, 0, sizeof(struct input_event));
	ev.type = EV_KEY;
	ev.code = key;
	ev.value = 0;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		die("error: write");

	memset(&ev, 0, sizeof(struct input_event));
	ev.type = EV_SYN;
	ev.code = 0;
	ev.value = 0;
	if(write(fd, &ev, sizeof(struct input_event)) < 0)
		die("error: write");

	return 0;
}

static int setup_dev()
{
	int fd;
	struct uinput_user_dev uidev;

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if(fd < 0)
		die("error: open");

	register_keys(fd, keys, sizeof(keys)/sizeof(int));

	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor  = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 1;

	if(write(fd, &uidev, sizeof(uidev)) < 0)
		die("error: write");

	if(ioctl(fd, UI_DEV_CREATE) < 0)
		die("error: ioctl");

	sleep(2);

	return fd;
}

static void cleanup_dev(int fd)
{
	if(ioctl(fd, UI_DEV_DESTROY) < 0)
		die("error: ioctl");

	close(fd);

}

int main(void)
{
	int fd;

	fd = setup_dev();

	while(1) {
		gen_keystroke(fd, KEY_VOLUMEUP);
		sleep(1);
		gen_keystroke(fd, KEY_VOLUMEDOWN);
		sleep(1);
	}

	cleanup_dev(fd);

	return 0;
}
