#ifndef _BKD_H
#define _BKD_H

int bkd_start_daemon(const char* dev);
void bkd_stop_daemon(void);
int bkd_get_current_count(void);

#endif
