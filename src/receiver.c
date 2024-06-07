#include "receiver.h"

void * process(char* _buff) { return NULL; }

Receiver* Receiver_new(char* _addr, u_int16_t _port, char* _proto)
{
    struct sockaddr_in addr;
    int socketfd;
    struct protoent *proto = getprotobyname(_proto);

    socketfd = socket(AF_INET, SOCK_RAW, proto->p_proto);
    if (socketfd == -1) handle_error("socket");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    inet_pton(AF_INET, _addr, &addr.sin_addr);

    if (bind(socketfd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) 
    {
        handle_error("bind");
    }

    printf("[*] %s Receiver binded on (%s, %hu)\n", proto->p_name, _addr, _port);

    Receiver* recv = (Receiver*)malloc(sizeof(Receiver));
    recv->_address = addr;
    recv->_proto = proto;
    recv->_socket = socketfd;
    recv->_running = false;
    recv->__process_fn = process;

    return recv;
}

void Receiver_delete(Receiver* _self)
{
    free(_self);
}

void *Receiver_run(void* _self)
{
    u_int32_t addrn = ((Receiver*)_self)->_address.sin_addr.s_addr;
    u_int16_t port = ((Receiver*)_self)->_address.sin_port;
    char* pname = ((Receiver*)_self)->_proto->p_name;

    char* addr = addressNumberToString(addrn, true);
    printf("[*] %s Receiver starting on (%s, %hu)\n", pname, addr, port);
    free(addr);

    size_t ip_size = (IP_PAYLOAD_MAX_SIZE + IP_HEADER_SIZE);
    socklen_t socklen = sizeof(((Receiver*) _self)->_address);
    char* buff = (char*)malloc(ip_size * sizeof(char));

    while (((Receiver*)_self)->_running)
    {
        // Receive a message from the socket
        ssize_t retval = recvfrom(
            ((Receiver*) _self)->_socket, buff, ip_size, MSG_DONTWAIT,
            (struct sockaddr*)&((Receiver*) _self)->_address, &socklen
        );
        
        if (retval < 0) continue;
        printf("[*] %s Receiver received %ld bytes\n", pname, retval);
        
        ((Receiver*)_self)->__process_fn(buff);
    }

    free(buff);
    printf("[*] Receiver stopped\n");
    return NULL;
}

void Receiver_start(Receiver* _self, void *(*__process_fn) (char *))
{
    _self->_running = true;
    _self->__process_fn = __process_fn;

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, Receiver_run, _self);
    pthread_detach(recv_thread);

    _self->_thread = recv_thread;
}

void Receiver_stop(Receiver* _self)
{
    _self->_running = false;
}