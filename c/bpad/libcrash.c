#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

static void crash2() { char x[16*1024]; memset(x, 0xaa, sizeof(x)); volatile int* a = (int*)(NULL); *a = 1; }
static void crash1() { char x[16*1024]; memset(x, 0xbb, sizeof(x)); crash2(); }

void* thread_func(void *arg)
{
	char buf[16384];
	if (!arg) {
		while (1) {
			memset(buf, 0xa5, sizeof(buf));
			sleep (1);
			memset(buf, 0x0, sizeof(buf));
		}
	} else {
		int *n = NULL;
		sleep (5);
		*n = 100;
	}
}
#define MAX_HANDLES	200
void crash()
{
	int i;
	pthread_t handles[MAX_HANDLES];

	for (i = 0; i < MAX_HANDLES; i++) {
		if (pthread_create(&handles[i], NULL, thread_func, (i == MAX_HANDLES-1) ? &i : NULL) == 0)
			printf("Thread %d created\n", i);
		else
			printf("Thread %d creation failed\n", i);
	}

	while (1) {
		sleep (1);
	}
}

#ifdef __cplusplus
}
#endif

