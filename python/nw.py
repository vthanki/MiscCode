#!/usr/bin/python

import socket
import struct
import sys
import binascii

#struct packet {
#	uint8_t magic;
#	uint8_t msg_type;
#	uint16_t msg_len;
#	uint8_t tid;
#	uint8_t data[0];
#};

hdr_fmt = struct.Struct('BBHB');

def discover_target():
    # Discovery packet
    pkt = (0x13, 0x0, 0x0000, 0x0);
    packed_pkt = hdr_fmt.pack(*pkt);
    s.sendto(packed_pkt, ("255.255.255.255", 8153))

    rcvd = s.recv(4096)
    hdr = rcvd[0:5]
    unpacked_data = hdr_fmt.unpack(hdr);
    payload = rcvd[5:]
    return payload;

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
s.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
s.bind(("0.0.0.0", 1765))
target = discover_target()
print "Target Device IP:", target.split(",")[0]
print "Target Device Model:", target.split(",")[1]
print "Target Device FW:", target.split(",")[2].strip()



