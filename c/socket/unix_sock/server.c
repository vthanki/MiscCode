/*
 ** echos.c -- the echo server for echoc.c; demonstrates unix sockets
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <poll.h>

#define SOCK_PATH "/tmp/unix.socket.ex"

int main(void)
{
	int s, s2, t, len;
	struct sockaddr_un local, remote;
	char str[100] = {0};
	struct pollfd pfd;

	if ((s = socket(AF_UNIX, SOCK_SEQPACKET | SOCK_CLOEXEC, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	local.sun_family = AF_UNIX;
	strcpy(local.sun_path, SOCK_PATH);
	unlink(local.sun_path);
	len = strlen(local.sun_path) + sizeof(local.sun_family);
	if (bind(s, (struct sockaddr *)&local, len) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(s, 5) == -1) {
		perror("listen");
		exit(1);
	}

	for(;;) {
		int done, n;
		printf("Waiting for a connection...\n");
		t = sizeof(remote);
		if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
			perror("accept");
			exit(1);
		}

		printf("Connected.\n");

		done = 0;
		do {
			int rc;
			printf("Sleeping for 5 seconds, connect the client and exit from client\n");
			sleep(5);
			pfd.fd = s2;
		        pfd.events = POLLIN;
			rc = poll(&pfd, 1, 15000);
			if (!rc) printf("poll timed out\n");
			if (rc < 0) printf("poll failed\n");
			printf("events: 0x%x\n", pfd.revents);
			if (pfd.revents & POLLIN) {
				n = recv(s2, str, 100, MSG_PEEK);
				printf("There are %d bytes to be read\n", n);
			}

		//	printf("1000 seconds sleep before receive\n");
		//	sleep (1000);
			n = recv(s2, str, 100, 0);
			if (n <= 0) {
				if (n < 0) perror("recv");
				done = 1;
			}
			printf("data recvd: %s\n", str);
			break;

			if (!done)
				if (send(s2, str, n, 0) < 0) {
					perror("send");
					done = 1;
				}
		} while (!done);

		close(s2);
	}

	return 0;
}
