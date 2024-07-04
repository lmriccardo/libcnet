#include "receiver.h"

Receiver* Receiver_new(
    const char* _interface, const u_int16_t _port, const char* _proto, const bool _verbose
) {
    struct sockaddr_in addr;
    int socketfd;
    struct protoent *proto = getprotobyname(_proto);

    char _addr[INET_ADDRSTRLEN];
    getInterfaceIp(_interface, _addr);

    socketfd = socket(AF_INET, SOCK_RAW, proto->p_proto);
    if (socketfd == -1) handle_error("socket", socketfd);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    inet_pton(AF_INET, _addr, &addr.sin_addr);

    if (bind(socketfd, (const struct sockaddr*)&addr, sizeof(addr)) < 0) 
    {
        handle_error("bind", socketfd);
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
    recv->_verbose = _verbose;
    recv->_timer = NULL;
    recv->_mutex = NULL;
    recv->_queue = MessageQueue_new(100);

    return recv;
}

void Receiver_delete(Receiver* _self)
{
    shutdown(_self->_socket, 2);
    MessageQueue_delete(_self->_queue);
    free(_self);
}

void *Receiver_run(void* _self)
{
    u_int32_t addrn = ((Receiver*)_self)->_address.sin_addr.s_addr;
    u_int16_t port = ((Receiver*)_self)->_address.sin_port;
    char* pname = ((Receiver*)_self)->_proto->p_name;
    
    char addr[INET_ADDRSTRLEN];
    addressNumberToString(addrn, addr, true);
    
    if (((Receiver*)_self)->_verbose)
    {
        printf("[*] %s Receiver starting on (%s, %hu)\n", pname, addr, ntohs(port));
    }

    size_t ip_size = (IP_PAYLOAD_MAX_SIZE + IP_HEADER_SIZE);
    socklen_t socklen = sizeof(((Receiver*) _self)->_address);
    char buff[ip_size];

    struct Response *resp = (struct Response*)malloc(sizeof(struct Response));
    resp->_buffer = (char *)malloc(ip_size * sizeof(char));
    double rtt;

    while (((Receiver*)_self)->_running)
    {

        if (((Receiver*)_self)->_mutex != NULL)
        {
            semaphore_wait(((Receiver*)_self)->_mutex, "Receiver_run");
        }

        // Receive a message from the socket
        ssize_t retval = recvfrom(
            ((Receiver*) _self)->_socket, buff, ip_size, MSG_DONTWAIT,
            (struct sockaddr*)&((Receiver*) _self)->_address, &socklen
        );
        
        if (retval < 0)
        {
            if (((Receiver*)_self)->_mutex != NULL)
            {
                semaphore_post(((Receiver*)_self)->_mutex, "Receiver_run");
            }

            continue;
        }

        if (((Receiver*)_self)->_verbose) 
        {
            printf("[*] %s Receiver received %ld bytes\n", pname, retval);
        }

        rtt = ((Receiver*)_self)->_timer != NULL ? Timer_getDeltaTime(((Receiver*)_self)->_timer) : 0.0;

        // Create the response
        memcpy(resp->_buffer, buff, retval);
        resp->_rtt = rtt;
        resp->_size = retval;

        // Push the response in the message queue
        MessageQueue_push(((Receiver*)_self)->_queue, resp, sizeof(struct Response));

        if (((Receiver*)_self)->_mutex != NULL)
        {
            semaphore_post(((Receiver*)_self)->_mutex, "Receiver_run");
        }
    }

    if (((Receiver*)_self)->_timer != NULL)
    {
        Timer_stop(((Receiver*)_self)->_timer);
    }

    Response_free(resp);

    if (((Receiver*)_self)->_verbose)  printf("[*] Receiver stopped\n");
    return NULL;
}

void Receiver_start(Receiver* _self)
{
    _self->_running = true;

    // Start the timer if is not NULL
    if (_self->_timer != NULL) Timer_start(_self->_timer);

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, Receiver_run, _self);
    pthread_detach(recv_thread);
}

void Receiver_stop(Receiver* _self)
{
    _self->_running = false;
    sleep(1.0);
}

void Receiver_setTimer(Receiver* _self, struct Timer* _timer)
{
    _self->_timer = _timer;
}

void Receiver_setSemaphore(Receiver* _self, sem_t* _mutex)
{
    _self->_mutex = _mutex;
}

void Response_free(struct Response* _self)
{
    free(_self->_buffer);
    free(_self);
}