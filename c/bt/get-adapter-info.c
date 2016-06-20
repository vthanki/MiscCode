#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc, char **argv)
{
	int dev_id, rc, i;
	struct hci_dev_info info;
	char addr[32];

	dev_id = hci_get_route(NULL);
	if (dev_id < 0) {
		printf("Error getting device id: %d\n", dev_id);
		return dev_id;
	}

	rc = hci_devinfo(dev_id, &info);
	if (rc < 0) {
		printf("Error getting device info: %d\n", rc);
		return rc;
	}
	
	rc = ba2str(&info.bdaddr, addr);
	if (rc < 0) return rc;

	printf("Device name:%s\n", info.name);
	printf("Device addr:%s\n", addr);
	return 0;
}
