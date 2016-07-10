#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

#define BT_RFCOMM_CH		2

int unregister_service(sdp_session_t *session)
{
	return sdp_close(session);
}

sdp_session_t *register_service(void)
{
	uint32_t service_uuid_int[] = { 0, 0, 0, 0xABCD };
	char uuid_str[MAX_LEN_UUID_STR];
	uint8_t rfcomm_channel = BT_RFCOMM_CH;
	const char *service_name = "BT echo server";
	const char *service_dsc = "A Bluetooth based sample echo server";
	const char *service_prov = "BTDev";
	int err = 0;
	sdp_session_t *session = 0;
	sdp_profile_desc_t profile;


	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;

	sdp_list_t *l2cap_list = 0, 
		   *rfcomm_list = 0,
		   *root_list = 0,
		   *proto_list = 0, 
		   *access_proto_list = 0,
		   *profiles = 0,
		   *svclass = 0;

	sdp_data_t *channel = 0, *psm = 0;

	sdp_record_t *record = sdp_record_alloc();

	// set the general service ID
	sdp_uuid128_create( &svc_uuid, &service_uuid_int );
	sdp_set_service_id( record, svc_uuid );
	svclass = sdp_list_append(NULL, &svc_uuid);
	sdp_set_service_classes(record, svclass);

	sdp_uuid2strn(&svc_uuid, uuid_str, sizeof(uuid_str));
	printf("[+] BT Echo Service UUID : %s\n", uuid_str);

	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups( record, root_list );

	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append( 0, &l2cap_uuid );
	proto_list = sdp_list_append( 0, l2cap_list );

	sdp_uuid2strn(&l2cap_uuid, uuid_str, sizeof(uuid_str));
	printf("[+] BT Echo L2CAP UUID : %s\n", uuid_str);

	// set rfcomm information
	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
	rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );
	sdp_list_append( rfcomm_list, channel );
	sdp_list_append( proto_list, rfcomm_list );

	sdp_uuid2strn(&rfcomm_uuid, uuid_str, sizeof(uuid_str));
	printf("[+] BT Echo RFCOMM UUID : %s\n", uuid_str);

	// attach protocol information to service record
	access_proto_list = sdp_list_append( 0, proto_list );
	sdp_set_access_protos( record, access_proto_list );

	profile.version = 0x0100;
	profiles = sdp_list_append(NULL, &profile);
	sdp_set_profile_descs(record, profiles);


	// set the name, provider, and description
	sdp_set_info_attr(record, service_name, service_prov, service_dsc);

	// connect to the local SDP server, register the service record, and 
	// disconnect
	session = sdp_connect( BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY );
	if (!session) {
		return session;
	}

	err = sdp_record_register(session, record, 0);
#if 0
	// cleanup
	sdp_data_free( channel );
	sdp_list_free( l2cap_list, 0 );
	sdp_list_free( rfcomm_list, 0 );
	sdp_list_free( root_list, 0 );
	sdp_list_free( access_proto_list, 0 );
#endif
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
