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

Please, take a look at [Notes On Development](Note.md) for the state of the project and future development.

## A simple example

Here is a simple example on how, you can create and sent an ICMP Echo message

```c
/*
    FILENAME: icmp.c
*/

#include <sender.h>
#include <ip.h>

int main(int argc, char **argv)
{
    char raddr[INET_ADDRSTRLEN];
    getHostnameIP(argv[2], raddr);

    char payload[18];
    generateRandomData(payload, 18);
    
    bool verbose = true;

    Sender *sender = Sender_new(argv[1], raddr, NULL, INADDR_ANY, "icmp", verbose);
    IpPacket* pckt = Sender_craftIcmp(sender, ICMP_ECHO_TYPE, ICMP_ECHO_CODE, payload, 18);

    if (verbose)
    {
        IpHeader_printInfo(&pckt->_iphdr);
        IcmpHeader_printInfo(&pckt->_payload._icmp->_icmphdr);
    }

    Sender_send(sender, pckt, 0.0);

    Sender_delete(sender);
    IpPacket_delete(pckt);

    return 0;
}

```

Now, with the following command you can compile the program

```bash
gcc -Wall -O3 -I<include-path> -o icmp icmp.c -L<lib-folder> -lcnet
```

Finally, you can run it

```
$ sudo ./icmp eth0 google.com

[*] Printing Header Information Fields
Ip Version: 4
Internet Header Length: 5
Distributed Service Code Point: 0
Explicit Congestion Network: 0
Total Length: 46
Identification: 28472
IP Flags (X D M): 010
Fragment Offset: 0
Time To Live: 128
IP Protocol: 1
Header Checksum: 0
Source Address: 172.19.11.1
Destination Address: 142.251.209.14

[*] Printing Information of the ICMP Header
ICMP Message Type: 8
ICMP Message Code: 0
ICMP Header Checksum: 38223
ICMP Header Identifier: 1
ICMP Header Sequence Number: 0

[*] Sent 46 bytes of IP Packet
```

Notice, this output is quite verbose since we have set the verbosity of the sender to `True`. 

The following image is wireshark confirming that the message has being sent and also received.

![alt text](docs/imgs/udp-wireshark-capture.png)

Notice that, the verbose mode of the sender also generates a new file in the current folder. This new file
contains the entire IP packet that has been sent.

```
$ xxd sent_28473_1_icmp.bin

00000000: 4500 002e 6f38 4000 8001 0000 ac13 0b01  E...o8@.........
00000010: 8efb d10e 0800 954f 0001 0000 d170 b548  .......O.....p.H
00000020: c152 b3cb d009 2e71 b1eb e7ea ce85       .R.....q......
```

> **The most important thing you need to remember when using this library is, when creating "objects" using the**
> **corresponding _new_ function, at the bottom of the program must always be the corresponding _delete_ function**
> **in order to freed the memory previously allocated.**

One last important note is the `sudo` prefix when running an executable using this library. Under the hood, since
we are crafting from zero IP packets, meaning the header and payload, the sockets of the sender and the receiver
are created using the `SOCK_RAW` option, which requires to the calling user the `CAP_NET_ADMIN` or `CAP_NET_RAW` 
capabilities. The fastest way to obtain this capility is being root, here it comes the `sudo` command. However, 
there are two more possibilities:

1. Set the SUID bit to the `udp` executable, using `sudo chmod u+s udp`
2. Set either the `CAP_NET_ADMIN` or `CAT_NET_RAW` capability for the current user when running.

In general, setting the SUID bit is not a good idea in terms of security, since it can leads to easy lateral
movement, once an attacker has gained access to your machine as a non-privileged user. 

This was just a simple example, you can find more complex ones into the `example` folder.

## Installation

The installation is quite easy since no third-party libraries are used, just the default ones. These are the requirements

- `git` to download this repository with the source code
- `gcc` to compile the source and create the libraries
- `make` to run `Makefile`s

Before installing, please run the following commands:

```
sudo apt-get update && apt-get install -y build-essential
sudo apt-get install -y git make gcc
```

Once you have all the pre-requisites installed, then you can proceed with the installation

```
$ git clone https://github.com/lmriccardo/libcnet.git
$ cd libcnet
$ sudo ./scripts/build.sh
```

The previous command will populate the `build` folder with

- `include` folder with all the header files
- `bin` with all the object files
- `static` containing the static library `libcnet.a`
- `shared` containing the shared library `libcnet.so`

## References

[1] [RFC 792](https://datatracker.ietf.org/doc/html/rfc792) for ICMP Packets

[2] [RFC 768](https://datatracker.ietf.org/doc/html/rfc768) for UDP Packets

[3] [RFC 791](https://datatracker.ietf.org/doc/html/rfc791) and [RFC 2474](https://datatracker.ietf.org/doc/html/rfc2474) for IP Packets

[4] [RFC 1191](https://datatracker.ietf.org/doc/html/rfc1191) for vanilla Path MTU Discovery Implementation

[5] *Linux Man Page* for various functionality with sockets, threads and so on.

## Conclusions

I hope that you like this library. Please free to test it and giving me feedback. If you like this library, please leave a star.