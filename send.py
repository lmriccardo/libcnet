import socket

s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

ip_header = b'\x45\x00\x00\x1c' # Version, IHL, Type of Service | Total Length
ip_header += b'\xab\xcd\x00\x00' # Identification | Flags, Fragment Offset
ip_header += b'\x40\x01\x6b\xd8' # TTL, Protocol | Header Checksum
ip_header += b'\xc0\xa8\x92\x83' # Source Address
ip_header += b'\x08\x08\x08\x08' # Destination Address

icmp_header = b'\x08\x00\xe5\xca' # Type of message, Code | Checksum
icmp_header += b'\x12\x34\x00\x01' # Identifier | Sequence Number

packet = ip_header + icmp_header
s.sendto(packet, ('8.8.8.8', 0))

# import socket

# s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
# s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)

# ip_header  = b'\x45\x00\x00\x28'  # Version, IHL, Type of Service | Total Length
# ip_header += b'\xab\xcd\x00\x00'  # Identification | Flags, Fragment Offset
# ip_header += b'\x40\x06\xa6\xec'  # TTL, Protocol | Header Checksum
# ip_header += b'\x0a\x0a\x0a\x02'  # Source Address
# ip_header += b'\x0a\x0a\x0a\x01'  # Destination Address

# tcp_header  = b'\x30\x39\x00\x50' # Source Port | Destination Port
# tcp_header += b'\x00\x00\x00\x00' # Sequence Number
# tcp_header += b'\x00\x00\x00\x00' # Acknowledgement Number
# tcp_header += b'\x50\x02\x71\x10' # Data Offset, Reserved, Flags | Window Size
# tcp_header += b'\xe6\x32\x00\x00' # Checksum | Urgent Pointer

# packet = ip_header + tcp_header
# s.sendto(packet, ('8.8.8.8', 0))