#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char **argv)
{
	int sockfd;
	int portno;
	int clientlen;
	struct sockaddr_in serveraddr;
	struct hostent *hostp;
	char buf[BUFSIZE] = {0};
	char *hostaddrp;
	int broadcast_en = 1;
	int n;

	if (argc != 4) {
		fprintf(stderr, "usage: %s <server> <port> <data-in-quotes>\n",
				argv[0]);
		exit(1);
	}

	portno = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	/*
	 * main loop: wait for a datagram, then echo it
	 */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons((unsigned short)portno);

	/* Set Broadcast permissions */
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_en, sizeof(broadcast_en));

	strncpy(buf, argv[3], sizeof(buf)-1);
	while (1) {

		printf("Sending :%s\n", buf);

		n = sendto(sockfd, buf, sizeof(buf), 0,
				(struct sockaddr *) &serveraddr, sizeof(serveraddr));
		if (n < 0)
			error("ERROR in sendto");
		sleep(2);
	}
}
