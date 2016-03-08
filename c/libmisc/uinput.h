#ifndef _LIBMISC_UINPUT_H
#define _LIBMISC_UINPUT_H

#ifdef __cplusplus
extern "C" {
#endif

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

int lm_gen_keystroke(int fd, int key);
int lm_setup_dev(int *keys, int num_keys);
void lm_cleanup_dev(int fd);

#ifdef __cplusplus
}
#endif
#endif /* _LIBMISC_UINPUT_H */
