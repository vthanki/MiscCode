#include <stdio.h>
#include <fcntl.h>

#include "br.h"

static struct br_client_sock sock;

void print_pkt_info(struct br_hdr *brhdr)
{
	printf("BR_PKT: op:		%d\n", brhdr->op);
	printf("BR_PKT: ack:		%d\n", brhdr->ack);
	printf("BR_PKT: chunk_id:	%d\n", brhdr->chunk.chunk_id);
	printf("BR_PKT: len:		%d\n", brhdr->len);
}


void setup_client_socket(void)
{
	int broadcast_en = 1;

	sock.sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock.sockfd < 0)
		error("ERROR opening socket");

	bzero((char *) &sock.saddr, sizeof(sock.saddr));
	sock.saddr.sin_family = AF_INET;
	sock.saddr.sin_addr.s_addr = inet_addr(BR_IP);
	sock.saddr.sin_port = htons((unsigned short)BR_PORTS);

	/* Set Broadcast permissions */
	setsockopt(sock.sockfd, SOL_SOCKET, SO_BROADCAST,
			&broadcast_en, sizeof(broadcast_en));

	bzero((char *) &sock.raddr, sizeof(sock.raddr));
	sock.raddr.sin_family = AF_INET;
	sock.raddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sock.raddr.sin_port = htons((unsigned short)BR_PORTC);

	/*
	 * bind: associate the parent socket with a port
	 */
	if (bind(sock.sockfd, (struct sockaddr *) &sock.raddr,
				sizeof(sock.raddr)) < 0)
		error("ERROR on binding");
}

int send_discover(void)
{
	struct sockaddr_in clientaddr;
	struct br_hdr *brhdr;
	int clientlen;
	char buf[MAX_BR_PKT_LEN] = {0};

	printf("Sending DISCOVER\n");

	brhdr = (struct br_hdr *) buf;
	brhdr->op = DISCOVER;

	if (sendto(sock.sockfd, buf, sizeof(buf), 0,
				(struct sockaddr *) &sock.saddr,
				sizeof(sock.saddr)) < 0)
		error("ERROR while sending DISCOVER");


	memset(buf, 0x0, sizeof(buf));


	clientlen = sizeof(clientaddr);
	if (recvfrom(sock.sockfd, buf, MAX_BR_PKT_LEN, 0,
			(struct sockaddr *) &clientaddr, &clientlen) < 0)
		error("ERROR in recvfrom");

	brhdr = (struct br_hdr *) buf;

	if (brhdr->op == DISCOVER && brhdr->ack) {
		printf("Server available in network\n");
		return BR_SERVER_FOUND;
	} else {
		printf("Server not available in network\n");
		return BR_SERVER_NOT_FOUND;
	}
}

void fetch_file_info(const char *fname, int *nr_chunks, int *last_chunk_sz)
{
	char buf[MAX_BR_PKT_LEN] = {0};
	struct br_hdr *brhdr = (struct br_hdr *) buf;
	struct sockaddr_in clientaddr;
	int clientlen;

	brhdr->op = FILE_INFO;
	brhdr->len = strlen(fname);

	strncpy(buf + sizeof(struct br_hdr), fname, brhdr->len);

	if (sendto(sock.sockfd, buf, sizeof(buf), 0,
				(struct sockaddr *) &sock.saddr,
				sizeof(sock.saddr)) < 0)
		error("ERROR while sending FILE_INFO");


	memset(buf, 0x0, sizeof(buf));

	clientlen = sizeof(clientaddr);
	if (recvfrom(sock.sockfd, buf, MAX_BR_PKT_LEN, 0,
			(struct sockaddr *) &clientaddr, &clientlen) < 0)
		error("ERROR in recvfrom");

	brhdr = (struct br_hdr *) buf;

	*nr_chunks = brhdr->chunk.nr_chunks;
	*last_chunk_sz = brhdr->len;

	print_pkt_info(brhdr);
}

void fetch_file(int fd, int nr_chunks, int last_chunk_sz)
{
	int i;
	char buf[MAX_BR_PKT_LEN];
	struct br_hdr *brhdr;
	struct sockaddr_in clientaddr;
	int clientlen;

	for (i = 0; i < nr_chunks; i++) {
retry:
		memset(buf, 0x0, sizeof(buf));
		brhdr = (struct br_hdr *)buf;
		brhdr->op = CHUNK_READ;
		brhdr->chunk.chunk_id = i;

		if (sendto(sock.sockfd, buf, sizeof(buf), 0,
					(struct sockaddr *) &sock.saddr,
					sizeof(sock.saddr)) < 0) {
			printf("ERROR while sending CHUNK_READ %d", i);
			goto retry;
		}

		memset(buf, 0x0, sizeof(buf));
		clientlen = sizeof(clientaddr);
		if (recvfrom(sock.sockfd, buf, MAX_BR_PKT_LEN, 0,
			(struct sockaddr *) &clientaddr, &clientlen) < 0) {
			printf("ERROR in recvfrom for CHUNK_READ %d", i);
			exit (-1);
		}

		brhdr = (struct br_hdr *) buf;
		if (brhdr->op == CHUNK_READ && brhdr->ack &&
				brhdr->chunk.chunk_id == i) {
			if (brhdr->len == last_chunk_sz) {
				printf("Last chunk. i:%d nr_chunks:%d\n",
						i, nr_chunks);
				/* Send last message to server indicating termination */
				brhdr = (struct br_hdr *)buf;
				brhdr->op = TRANSFER_DONE;
				sendto(sock.sockfd, buf, sizeof(buf), 0,
						(struct sockaddr *) &sock.saddr,
						sizeof(sock.saddr));
			}
			if(write(fd, buf+sizeof(*brhdr), brhdr->len) < 0)
				printf("Error in writing to file\n");


		} else {
			/* FIXME: Handle this condition, retry?!! */
			printf("Invalid CHUNK_READ response\n");
			print_pkt_info(brhdr);
		}
	}
}

int main(int argc, char **argv)
{
	int nr_chunks = 0, last_chunk_sz = 0, fd;
	char *fname = "uImage";

	setup_client_socket();

	if (send_discover() == BR_SERVER_FOUND) {
		fetch_file_info(fname, &nr_chunks, &last_chunk_sz);

		if (nr_chunks > 0) {
			fd = open(fname, O_RDWR | O_CREAT);
			if (fd < 0)
				error("ERROR in opening file");

			fetch_file(fd, nr_chunks, last_chunk_sz);
			close(fd);
		}
	}

	return 0;

}
