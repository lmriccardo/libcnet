import socket
import os

os.chdir("./test")

file = "sent_60627_1_icmp.bin"
IP_HEADER_SIZE = 20

content = open(file, mode="rb").read()
icmp_packet = content[IP_HEADER_SIZE:]
counter = len(icmp_packet)
# counter = 8
toint = lambda x: int.from_bytes(x, 'big', signed=False)

checksum = 0

def calculate_checksum(data):
    checksum = 0

    # Handle odd-length data
    print(len(data))
    if len(data) % 2 != 0:
        data += b"\x00"

    # Calculate checksum
    for i in range(0, len(data), 2):
        if i == 2: continue
        checksum += (data[i] << 8) + data[i+1]
        # checksum += int(data[i:i+2].hex(), 16)

    print(checksum, checksum >> 16, checksum & 0xffff)
    checksum = (checksum >> 16) + (checksum & 0xffff)
    print(checksum, checksum >> 16)
    checksum += checksum >> 16
    print(checksum)

    return (~checksum) & 0xffff

print(f"Checksum: {hex(calculate_checksum(icmp_packet))}")