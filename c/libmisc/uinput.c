#include "uinput.h"

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

int lm_gen_keystroke(int fd, int key)
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

int lm_setup_dev(int *keys, int num_keys)
{
	int fd;
	struct uinput_user_dev uidev;

	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if(fd < 0)
		die("error: open");

	register_keys(fd, keys, num_keys);

	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "libmisc-uinput");
	uidev.id.bustype = BUS_VIRTUAL;

	if(write(fd, &uidev, sizeof(uidev)) < 0)
		die("error: write");

	if(ioctl(fd, UI_DEV_CREATE) < 0)
		die("error: ioctl");

	sleep(2);

	return fd;
}

void lm_cleanup_dev(int fd)
{
	if(ioctl(fd, UI_DEV_DESTROY) < 0)
		die("error: ioctl");

	close(fd);

}
