import socket

s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_ICMP)
# s.setsockopt(socket.SOL_SOCKET, 25, str("eth0" + '\0').encode('utf-8'))

s.bind(("172.19.11.1", socket.INADDR_ANY))

while 1:
    data = s.recvfrom(1024)
    print(data)