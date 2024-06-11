#include "receiver.h"

void * __process(char* _buff, size_t _len) { return NULL; }

Receiver* Receiver_new(
    const char* _interface, const u_int16_t _port, const char* _proto, const bool _verbose
) {
    struct sockaddr_in addr;
    int socketfd;
    struct protoent *proto = getprotobyname(_proto);

    char *_addr = (char*)malloc(INET_ADDRSTRLEN * sizeof(char));
    getInterfaceIp(_interface, _addr);

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

    if (_verbose)
    {
        printf("[*] %s Receiver binded on (%s, %hu)\n", proto->p_name, _addr, _port);
    }

    Receiver* recv = (Receiver*)malloc(sizeof(Receiver));
    recv->_address = addr;
    recv->_proto = proto;
    recv->_socket = socketfd;
    recv->_running = false;
    recv->__process_fn = NULL;
    recv->_verbose = _verbose;
    recv->_timer = NULL;

    free(_addr);

    return recv;
}

void Receiver_delete(Receiver* _self)
{
    if (_self->_timer != NULL) free(_self->_timer);
    shutdown(_self->_socket, 2);
    free(_self);
}

void *Receiver_run(void* _self)
{
    u_int32_t addrn = ((Receiver*)_self)->_address.sin_addr.s_addr;
    u_int16_t port = ((Receiver*)_self)->_address.sin_port;
    char* pname = ((Receiver*)_self)->_proto->p_name;

    char* addr = addressNumberToString(addrn, true);
    
    if (((Receiver*)_self)->_verbose) 
    {
        printf("[*] %s Receiver starting on (%s, %hu)\n", pname, addr, ntohs(port));
    }

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
        if (((Receiver*)_self)->_verbose) printf("[*] %s Receiver received %ld bytes\n", pname, retval);
        ((Receiver*)_self)->__process_fn(buff, retval);
    }

    free(buff);
    if (((Receiver*)_self)->_verbose)  printf("[*] Receiver stopped\n");
    return NULL;
}

void Receiver_start(Receiver* _self, void *(*__process_fn) (char *, size_t))
{
    _self->_running = true;
    _self->__process_fn = __process_fn;

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, Receiver_run, _self);
    pthread_detach(recv_thread);
}

void Receiver_stop(Receiver* _self)
{
    _self->_running = false;
    sleep(0.75);
}

void Receviver_setTimer(Receiver* _self, struct Timer* _timer)
{
    _self->_timer = _timer;
}