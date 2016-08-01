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

#define SPORT		8153
#define CPORT		1765
#define BUFSIZE 	1024
#define PROTO_MAGIC	0x13
#define MAX_PKT_LEN	4096
#define ETHR_IFACE	"eno1"
#define FW_PATH		"/tmp/version"

uint8_t glb_tid;

enum msg_types {
	DISCOVERY,
	ANNOUNCEMENT,
	CMD_REQ,
	CMD_REP,
	EXE_REQ,
	EXE_REP,
	TERMINATE,
	INVALID,
};

enum cmd_types {
	FW_VER_REQ,
	MODEL_ID_REQ,
	PING_REQ,
};

struct packet {
	uint8_t magic;
	uint8_t msg_type;
	uint16_t msg_len;
	uint8_t tid;
	uint8_t data[0];
};

struct _announce_payload {
	unsigned char info[128];
};

/*
 * error - wrapper for perror
 */
void error(char *msg) {
	perror(msg);
	exit(1);
}

static const char *get_local_ip(const char *iface)
{
	int fd;
	struct ifreq ifr;

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		error("[-]: opening socket to get IP");

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

	if (ioctl(fd, SIOCGIFADDR, &ifr) < 0)
		error("[-]: failed to get IP");

	close(fd);

	return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

static void get_fw_ver(char *fw_ver)
{
	FILE *fp;

	fp = popen("cat /tmp/version", "r");
	if (fp == NULL)
		error("[-]: Failed to get fw version\n" );
	fgets(fw_ver, 32, fp);
	pclose(fp);
}

static int start_server(int port, const char *server_ip)
{
	int sockfd;
	struct sockaddr_in serveraddr;
	int optval;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		error("[-]: opening socket");

	/* setsockopt: Handy debugging trick that lets
	 * us rerun the server immediately after we kill it;
	 * otherwise we have to wait about 20 secs.
	 * Eliminates "ERROR on binding: Address already in use" error.
	 */

	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
			(const void *)&optval , sizeof(int));


	/* Set Broadcast permissions */
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
			(const void *)&optval , sizeof(int));
	/*
	 * build the server's Internet address
	 */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(server_ip);
	serveraddr.sin_port = htons((unsigned short)port);

	/*
	 * bind: associate the parent socket with a port
	 */
	if (bind(sockfd, (struct sockaddr *) &serveraddr,
				sizeof(serveraddr)) < 0)
		error("[-]: binding");

	return sockfd;
}

static void make_announcement(int sockfd, const char *target)
{
	const char *ip = get_local_ip("eno1");
	char fw_ver[32] = {0};
	int model_id = 16; /* Hardcoded to soundbar */
	struct _announce_payload payload;
	struct packet *pkt;
	struct sockaddr_in saddr;

	pkt = alloca(sizeof(*pkt) + sizeof(payload));
	if (!pkt)
		error("[-]: Error allocating memory for announcement pkt\n");

	pkt->magic = PROTO_MAGIC;
	pkt->msg_type = ANNOUNCEMENT;
	pkt->msg_len = sizeof(payload);
	pkt->tid = glb_tid++;

	get_fw_ver(fw_ver);
	fw_ver[strlen(fw_ver)-1] = '\0';
	memset(payload.info, 0x0, sizeof(payload.info));
	snprintf(payload.info, 128, "%s,%d,%s",	ip, model_id, fw_ver);
	printf("[*]: announcment:%s\n", payload.info);

	memcpy(pkt->data, payload.info, sizeof(payload.info));

	bzero((char *) &saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(target);
	saddr.sin_port = htons((unsigned short) CPORT);

	if (sendto(sockfd, pkt, sizeof(*pkt) + sizeof(payload), 0,
				(struct sockaddr *) &saddr,
				sizeof(saddr)) < 0)
		error("ERROR while acking DISCOVER");
	printf("[*]: packet sent\n");
}

int process_pkt(int sockfd)
{
	int bytes_rcvd;
	int clientlen;
	int quit = 0, ret = 0;
	char buf[MAX_PKT_LEN];
	char *hostaddrp;

	struct sockaddr_in clientaddr;
	struct packet *pkt;

	clientlen = sizeof(clientaddr);
	while (!quit) {

		bzero(buf, MAX_PKT_LEN);
		bytes_rcvd = recvfrom(sockfd, buf, MAX_PKT_LEN, 0,
				(struct sockaddr *) &clientaddr, &clientlen);
		if (bytes_rcvd < 0)
			error("[-]: recvfrom");

		hostaddrp = inet_ntoa(clientaddr.sin_addr);
		if (hostaddrp == NULL)
			error("[-]: inet_ntoa\n");

		pkt = (struct packet *) buf;

		if (pkt->magic != PROTO_MAGIC) {
			printf("[-]: Invalid protocol ID:%x\n", pkt->magic & 0xFF);
			continue;
		}

		switch (pkt->msg_type) {
			case DISCOVERY:
				printf("[+]: Discovery request from :%s\n",
						hostaddrp);
				glb_tid = pkt->tid;
				make_announcement(sockfd, hostaddrp);
				continue;
			case CMD_REQ ... TERMINATE:
			       printf("[-]: Message type %d not implemented\n",
					       pkt->msg_type);
			       break;
			default:
			       printf("[-]: Invald Message type %d\n",
					       pkt->msg_type);
				quit = 1;
				ret = -1;
				break;
		}
	}

	return ret;
}


int main(int argc, char **argv)
{
	int sockfd;
	const char *server_ip;

	if (argv[1])
		server_ip = argv[1];
	else
		server_ip = "0.0.0.0";

	sockfd = start_server(SPORT, server_ip);

	while (process_pkt(sockfd))
		;

	return 0;

}
