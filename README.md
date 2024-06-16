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