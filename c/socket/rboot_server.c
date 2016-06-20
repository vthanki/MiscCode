/* 
 * udpserver.c - A simple UDP echo server 
 * usage: udpserver <port>
 */

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

struct RBoot_t {
	int rb_op;		/* Operation			*/
	int rb_img_size;
	int img_pkt_id;		/* Transaction ID		*/
	int temp;
};

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
	struct sockaddr_in clientaddr;
	struct hostent *hostp;
	char buf[BUFSIZE];
	char *hostaddrp;
	int optval;
	int n;
	struct RBoot_t *rb_pkt;

	struct ifreq ifr;
	if (argc != 3) {
		fprintf(stderr, "usage: %s <port> <useconds>\n", argv[0]);
		exit(1);
	}

	portno = atoi(argv[1]);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	/* setsockopt: Handy debugging trick that lets
	 * us rerun the server immediately after we kill it;
	 * otherwise we have to wait about 20 secs.
	 * Eliminates "ERROR on binding: Address already in use" error.
	 */

	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
			(const void *)&optval , sizeof(int));

	/*
	 * build the server's Internet address
	 */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)portno);

	/*
	 * bind: associate the parent socket with a port
	 */
	if (bind(sockfd, (struct sockaddr *) &serveraddr,
				sizeof(serveraddr)) < 0)
		error("ERROR on binding");

	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,  &optval, sizeof(optval));

	/*
	 * main loop: wait for a datagram, then echo it
	 */
	clientlen = sizeof(clientaddr);

	/*
	 * recvfrom: receive a UDP datagram from a client
	 */
	bzero(buf, BUFSIZE);
	n = recvfrom(sockfd, buf, BUFSIZE, 0,
			(struct sockaddr *) &clientaddr, &clientlen);
	if (n < 0)
		error("ERROR in recvfrom");

	hostaddrp = inet_ntoa(clientaddr.sin_addr);
	if (hostaddrp == NULL)
		error("ERROR on inet_ntoa\n");
	rb_pkt = buf;
	printf("server received datagram from (%s)\n", hostaddrp);
	printf("server received %d/%d bytes:\n", strlen(buf), n);
	printf("RB operation:%d\n", rb_pkt->rb_op);

	while (1) {
		/*
		 * sendto: echo the input back to the client
		 */
		rb_pkt->rb_op = 2;
		rb_pkt->img_pkt_id++;
		//clientaddr.sin_addr.s_addr = inet_addr("172.16.0.255");
		clientaddr.sin_addr.s_addr = inet_addr("255.255.255.255");
		clientaddr.sin_port = htons((unsigned short)8153);
		n = sendto(sockfd, buf, sizeof(buf), 0,
				(struct sockaddr *) &clientaddr, clientlen);
		if (n < 0)
			error("ERROR in sendto");
		if (atoi(argv[2]))
			usleep(atoi(argv[2]));
	}
}
