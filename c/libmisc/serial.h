#ifndef _LIBMISC_SERIAL_H
#define _LIBMISC_SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int lm_set_tty_attr(int fd, int speed, int parity);

#ifdef __cplusplus
}
#endif
#endif /* _LIBMISC_SERIAL_H */
