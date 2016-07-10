#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

#define BT_RFCOMM_CH		2

static bdaddr_t interface;
int unregister_service(sdp_session_t *session)
{
	return sdp_close(session);
}

sdp_session_t *register_service(void)
{
	sdp_list_t *svclass_id, *apseq, *proto[2], *profiles, *root, *aproto;
	uuid_t root_uuid, sp_uuid, l2cap, rfcomm;
	sdp_profile_desc_t profile;
	sdp_record_t record;
	uint8_t u8 = 1;
	sdp_data_t *channel;
	int ret = 0;
	sdp_session_t *session = 0;

	memset(&record, 0, sizeof(sdp_record_t));
	record.handle = 0x01020304;
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups(&record, root);

	sdp_uuid16_create(&sp_uuid, 0xABCD);
	svclass_id = sdp_list_append(0, &sp_uuid);
	sdp_set_service_classes(&record, svclass_id);

	sdp_uuid16_create(&profile.uuid, 0xABCD);
	profile.version = 0x0100;
	profiles = sdp_list_append(0, &profile);
	sdp_set_profile_descs(&record, profiles);

	sdp_uuid16_create(&l2cap, L2CAP_UUID);
	proto[0] = sdp_list_append(0, &l2cap);
	apseq = sdp_list_append(0, proto[0]);

	sdp_uuid16_create(&rfcomm, RFCOMM_UUID);
	proto[1] = sdp_list_append(0, &rfcomm);
	channel = sdp_data_alloc(SDP_UINT8, &u8);
	proto[1] = sdp_list_append(proto[1], channel);
	apseq = sdp_list_append(apseq, proto[1]);

	aproto = sdp_list_append(0, apseq);
	sdp_set_access_protos(&record, aproto);

	sdp_add_lang_attr(&record);

	sdp_set_info_attr(&record, "BT Echo Server", "VBT", "BT Port");

	sdp_set_service_id(&record, sp_uuid);
	sdp_set_service_ttl(&record, 0xffff);
	sdp_set_service_avail(&record, 0xff);
	sdp_set_record_state(&record, 0x00001234);

	session = sdp_connect( BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY );
	if (!session) {
		return session;
	}

	if (sdp_device_record_register(session, &interface, &record, SDP_RECORD_PERSIST) < 0) {
		printf("Service Record registration failed\n");
		ret = -1;
		goto end;
	}

end:
	sdp_data_free(channel);
	sdp_list_free(proto[0], 0);
	sdp_list_free(proto[1], 0);
	sdp_list_free(apseq, 0);
	sdp_list_free(aproto, 0);
	sdp_list_free(root, 0);
	sdp_list_free(svclass_id, 0);
	sdp_list_free(profiles, 0);

	return session;

}

int main(int argc, char **argv)
{
	struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
	char buf[1024] = { 0 };
	int s, client, bytes_read, bytes_write;
	socklen_t opt = sizeof(rem_addr);
	sdp_session_t *session = 0;

	// Register a service first
	session = register_service();
	if (!session) {
		fprintf(stderr, "[-] Failed to register BT Echo service\n");
		return -1;
	}

	printf("[+] BT Echo service registered successfully ...\n");

	// allocate socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

	// bind socket to port 1 of the first available 
	// local bluetooth adapter
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = *BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) 1;
	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

	// put socket into listening mode
	listen(s, 1);

	printf("[+] Waiting for connections ...\n");

	// accept one connection
	client = accept(s, (struct sockaddr *)&rem_addr, &opt);

	ba2str( &rem_addr.rc_bdaddr, buf );
	fprintf(stderr, "[+] Accepted connection from %s\n", buf);

	while (1) {
		memset(buf, 0, sizeof(buf));

		// read data from the client
		bytes_read = read(client, buf, sizeof(buf));
		if( bytes_read > 0 ) {
			printf("[+] received [%s]\n", buf);
		}

		bytes_write = write(client, buf, bytes_read);
		if (bytes_write <= 0) {
			printf("[-] Error %d in writing buf. Closing connection\n",
					bytes_write);
			break;
		}
	}
	// close connection
	close(client);
	close(s);

	// unregister service
	unregister_service(session);

	return 0;
}
