#include <execinfo.h>

static void dump_backtrace()
{
	int j, nptrs;
#define SIZE 100
	void *buffer[100];
	char **strings;
	int fd = open("/tmp/backtrace", O_RDWR | O_CREAT | 0644);
	if (fd > 0) {

		nptrs = backtrace(buffer, SIZE);
		printf("backtrace() returned %d addresses\n", nptrs);

		/* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
		   would produce similar output to the following: */

		strings = backtrace_symbols_fd(buffer, nptrs);
		if (strings == NULL) {
			perror("backtrace_symbols");
			exit(EXIT_FAILURE);
		}

		for (j = 0; j < nptrs; j++) {
			printf("%s\n", strings[j]);
			write(fd, strings[j], strlen(strings[j]));
			write(fd, "\n", 1);
		}

		free(strings);
		close(fd);
	}
}
