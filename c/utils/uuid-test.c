/*
 * Extract the UUID from a json file which has following
 * "deviceId":"<UUID>"
 * node.
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

char uuid[37];

char *find_uuid(const char *json)
{

	const char *devid;
	int uuid_starts = 0;
	if (!json)
		return NULL;

	if (!(devid = strstr(json, "deviceId\":"))) {
		printf("deviceID in not found in JSON\n");
		return NULL;
	}

	while (*devid) {
		if (*devid != ':' && !uuid_starts) {
			devid++;
			continue;
		}

		if (*devid == ':' && *(devid+1) == '"') {
			devid+=2;
			uuid_starts = 1;
		}

		/* read 36 chars of UUID */
		strncpy(uuid, devid, sizeof(uuid)-1);
		break;
	}
	return uuid;
}

int main(int argc, char *argv[])
{
	char buf[256] = {0};
	int fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		printf("error opening file :%d\n", fd);
		return fd;
	}

	if (read(fd, buf, 256) <= 0)
		printf("Error in read\n");

	close(fd);

	printf("uuid:%s\n", find_uuid(buf) ? : "NO UUID FOUND!!!");

	return 0;
}

