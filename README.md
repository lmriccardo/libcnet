# libcnet - A C-based library for TCP/IP Networking

> **Warning: This project is still under development. It might contains some bugs. Use it carefully**

Welcome to *libcnet* a simple lightweight high-level C library (for GNU Linux) for interacting with Sockets and IP Networking. The main goal
developing this library was to keep an high-level OOP-like approach, implementing several functionalities like: crafting your 
own IP packets from scratch, sending IP Packets through sockets, receiving packets from the network, syncrhonizing 
the sender and the receiver and more on. 

I would like you to notice that this is my first C project, there might be some bugs that will be fixed and memory leaks,
altough I check for memory safetiness with *valgrind* every time a new functionality is implemented. Notice also, that some
memory leaks might be caused by the thread(s) used with the Receiver. 

As I already said, the main goal of this library, despite IP and socket programming, is to keep things as high-level
as possible, which means no deeply technical knowledge is required. However, also more low-level functionalities are
provided. In other words, you can let us decide or do it in your own. 

## A simple example

Here is a simple example on how, you can create and sent an UDP message

```c
/*
    FILENAME: udp.c
*/

#include <utils/net.h>
#include <sender.h>

#define BUFF_SIZE 64

// Assume argv[1] -> interface (like eth0)
// Assume argv[2] -> hostname (like google.com)
int main(int argc, char **argv)
{
    // Create some usefull variables
    char remote[INET_ADDRSTRLEN], payload[BUFF_SIZE];
    u_int16_t srcport = INADDR_ANY, dstport = 1234;

    // Obtain the IP of the remote host
    getHostnameIP(argv[2], remote);

    // Create the sender
    Sender* sender = Sender_new(argv[1], remote, NULL, dstport, "udp", true);

    // Generate a random payload
    generateRandomData(BUFF_SIZE, payload);

    // Send the UDP Message
    Sender_sendUdp(sender, srcport, payload, BUFF_SIZE);

    // Free the allocated memory
    Sender_delete(sender);

    return 0;
}
```

Now, with the following command you can compile the program

```bash
gcc -Wall -O3 -I<include-path> -o udp udp.c -L<lib-folder> -lcnet
```

Finally, you can run it

```
$ sudo ./udp eth0 google.com

[*] Printing Header Information Fields
Ip Version: 4
Internet Header Length: 5
Distributed Service Code Point: 0
Explicit Congestion Network: 0
Total Length: 92
Identification: 31372
IP Flags (X D M): 010
Fragment Offset: 0
Time To Live: 128
IP Protocol: 17
Header Checksum: 0
Source Address: 172.20.60.222
Destination Address: 142.251.209.46

[*] Printing Header Information Fields
Source Port: 0
Destination Port: 1234
Length: 72
Checksum: 41090

[*] Sent 92 bytes of IP Packet
```

Notice, this output is quite verbose since we have set the verbosity of the sender to `True`. 

The following image is wireshark confirming that the message has being sent.

![alt text](docs/imgs/udp-wireshark-capture.png)