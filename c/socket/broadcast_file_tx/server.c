#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "br.h"

static int fd = -1;
int start_server(int port)
{
	int sockfd;
	struct sockaddr_in serveraddr;
	int optval;

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


	/* Set Broadcast permissions */
	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST,
			(const void *)&optval , sizeof(int));
	/*
	 * build the server's Internet address
	 */
	bzero((char *) &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	/*
	 * bind: associate the parent socket with a port
	 */
	if (bind(sockfd, (struct sockaddr *) &serveraddr,
				sizeof(serveraddr)) < 0)
		error("ERROR on binding");

	return sockfd;
}

void print_pkt_info(struct br_hdr *brhdr)
{
	printf("BR_PKT: op:		%d\n", brhdr->op);
	printf("BR_PKT: ack:		%d\n", brhdr->ack);
	printf("BR_PKT: chunk_id:	%d\n", brhdr->chunk.chunk_id);
	printf("BR_PKT: len:		%d\n", brhdr->len);
}

void process_discover(int sockfd)
{
	struct sockaddr_in saddr;
	struct br_hdr brhdr = {
		.op = DISCOVER,
		.ack = 1,
		.len = sizeof(struct br_hdr),
	};

	bzero((char *) &saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(BR_IP);
	saddr.sin_port = htons((unsigned short)BR_PORTC);

	sleep (2);

	if (sendto(sockfd, &brhdr, sizeof(brhdr), 0,
				(struct sockaddr *) &saddr,
				sizeof(saddr)) < 0)
		error("ERROR while acking DISCOVER");
}

void process_fileinfo(int sockfd, void *buf)
{
	struct br_hdr *brhdr = (struct br_hdr *)buf;
	struct br_hdr brhdr_rep;
	int fname_offset = sizeof(*brhdr);
	int fname_len = brhdr->len;
	struct stat file_stats;
	struct sockaddr_in saddr;

	char *fname = strndup(buf + fname_offset, fname_len);

	if (!fname)
		error("process_fileinfo: ERROR in allocating memory");

	fd = open(fname, O_RDONLY);
	if (fd < 0) {
		printf("Error %d in opening requested file %s\n", fd, fname);
	}

	if (fd > 0 && fstat(fd, &file_stats) < 0) {
		printf("Error in reading stats of file %s\n", fname);
		goto exit;
	}

	brhdr_rep.op = FILE_INFO;
	brhdr_rep.ack = 1;
	
	if (fd > 0) {
		brhdr_rep.chunk.nr_chunks = file_stats.st_size / MAX_BR_PAYLOAD_LEN;
		if (file_stats.st_size % MAX_BR_PAYLOAD_LEN) {
			brhdr_rep.chunk.nr_chunks++;
			brhdr_rep.len = file_stats.st_size % MAX_BR_PAYLOAD_LEN;
		}

		printf("Size of %s file is %d, total chunks %d\n",
				fname, file_stats.st_size, brhdr_rep.chunk.nr_chunks);
	} else {
		brhdr_rep.chunk.nr_chunks = -1;
	}

	bzero((char *) &saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(BR_IP);
	saddr.sin_port = htons((unsigned short)BR_PORTC);

	if (sendto(sockfd, &brhdr_rep, sizeof(brhdr_rep), 0,
				(struct sockaddr *) &saddr,
				sizeof(saddr)) < 0)
		error("ERROR while acking FILE_INFO");

exit:
	free(fname);
}

void process_chunkrd(int sockfd, struct br_hdr *hdr)
{
	/* Once file transfer starts, no other packets
	 * will be entertained, and this routine only
	 * exits once the entire file transfer is over.
	 */
	char buf[MAX_BR_PKT_LEN];
	struct br_hdr *brhdr_rep;
	struct br_hdr *brhdr = hdr;
	struct sockaddr_in saddr;
	struct sockaddr_in clientaddr;
	int clientlen;
	int bytes_read;
	int last_chunk_id;

	bzero((char *) &saddr, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(BR_IP);
	saddr.sin_port = htons((unsigned short)BR_PORTC);


	while ((bytes_read = read(fd, buf+sizeof(struct br_hdr), MAX_BR_PAYLOAD_LEN)) > 0) {
		brhdr_rep = (struct br_hdr *) buf;
		brhdr_rep->op = CHUNK_READ;
		brhdr_rep->ack = 1;
		brhdr_rep->chunk.chunk_id = brhdr->chunk.chunk_id;
		brhdr_rep->len = bytes_read;
		last_chunk_id = brhdr->chunk.chunk_id;

		if (sendto(sockfd, buf, sizeof(buf), 0,
					(struct sockaddr *) &saddr,
					sizeof(saddr)) < 0) {
			printf("ERROR in sending CHUNK_READ ack :%d\n",
					brhdr->chunk.chunk_id);
			/* FIXME: Handle this??!! */
		}

		bzero(buf, MAX_BR_PKT_LEN);
		clientlen = sizeof(clientaddr);
		if (recvfrom(sockfd, buf, MAX_BR_PKT_LEN, 0,
			(struct sockaddr *) &clientaddr, &clientlen) < 0) {
			printf("ERROR in receiving next CHUNK_READ \n");
			/* FIXME: Handle this??!! */
		}

		brhdr = (struct br_hdr *) buf;
		if (brhdr->op == TRANSFER_DONE) {
			break;
		} else if (brhdr->op != CHUNK_READ) {
			printf("Non CHUNK_READ request in the middle\n");
			print_pkt_info(brhdr);
		} else if (brhdr->chunk.chunk_id != last_chunk_id + 1) {
			printf("OOO CHUNK_READ request arrived: Expected chunk:%d, Requested chunk:%d\n",
					last_chunk_id + 1,
					brhdr->chunk.chunk_id);
		}
	}
	printf("All chunks read from the file\n");
}

int process_pkt(int sockfd)
{
	int bytes_rcvd;
	int clientlen;
	int quit = 0, ret = 0;
	char buf[MAX_BR_PKT_LEN];
	char *hostaddrp;

	struct sockaddr_in clientaddr;
	struct br_hdr *brhdr;

	clientlen = sizeof(clientaddr);
	while (!quit) {

		bzero(buf, MAX_BR_PKT_LEN);
		bytes_rcvd = recvfrom(sockfd, buf, MAX_BR_PKT_LEN, 0,
				(struct sockaddr *) &clientaddr, &clientlen);
		if (bytes_rcvd < 0)
			error("ERROR in recvfrom");

		hostaddrp = inet_ntoa(clientaddr.sin_addr);
		if (hostaddrp == NULL)
			error("ERROR on inet_ntoa\n");

		brhdr = (struct br_hdr *) buf;

		switch (brhdr->op) {
			case DISCOVER:
				process_discover(sockfd);
				continue;
			case FILE_INFO:
				process_fileinfo(sockfd, (void *)buf);
				break;
			case CHUNK_READ:
				process_chunkrd(sockfd, brhdr);
				break;
			default:
				printf("Invalid BR opcode :%d\n", brhdr->op);
				quit = 1;
				ret = -1;
				break;
		}
	}

	return ret;
}

int main(int argc, char *argv[])
{
	int sockfd;

	sockfd = start_server(BR_PORTS);

	while (process_pkt(sockfd))
		;

	return 0;
}
