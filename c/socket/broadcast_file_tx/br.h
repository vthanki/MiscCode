#ifndef _BR_H
#define _BR_H

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

enum br_proto_ops {
	DISCOVER,
	FILE_INFO,
	CHUNK_READ,
	TRANSFER_DONE,
};

enum br_discovery_state {
	BR_SERVER_FOUND,
	BR_SERVER_NOT_FOUND,
};

struct br_hdr {
	int op;
	int ack;
	union {
	       int chunk_id;
	       int nr_chunks;
	} chunk;
	int len;
};

struct br_client_sock {
	int sockfd;
	struct sockaddr_in saddr;
	struct sockaddr_in raddr;
};

#define MAX_BR_PKT_LEN		(1024)
#define MAX_BR_PAYLOAD_LEN	(1024 - sizeof(struct br_hdr))

#define BR_PORTS	1765
#define	BR_PORTC	8153

#define BR_IP		"255.255.255.255"

/*
 * error - wrapper for perror
 */
static inline void error(char *msg)
{
	perror(msg);
	exit(1);
}

#endif
