from scapy.all import *
from itertools import cycle

def xorr(str, key):
    tmp = ""
    for i, j in zip(str, cycle(key)):
        tmp += chr(i ^ j)
    return tmp

CIRC_packet = []
def handle_packet(packet):
    if TCP in packet and Raw in packet:
        payload = packet[Raw].load
        if b"CIRC" in payload:
            # print(f"Payload: {payload}")
            CIRC_packet.append(payload)

filename = "../deploy/spy_report.pcapng"
pcap = rdpcap(filename)

for packet in pcap:
    handle_packet(packet)

CIRC_packet = CIRC_packet[1:]

key_1 = CIRC_packet[2][-16:]
key_2 = CIRC_packet[3][-16:]

shared_key = []

for i in range(8):
    shared_key.append(key_1[i] ^ key_2[i+8])

for i in range(8, 16):
    shared_key.append(key_1[i] ^ key_2[i-8])

print(shared_key)

for i in CIRC_packet[4:]:
    print(xorr(i[20:], shared_key))