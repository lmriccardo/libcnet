#ifndef _RECEIVER_H
#define _RECEIVER_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>
#include "neto.h"

__BEGIN_DECLS

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct 
{

    struct protoent*   _proto;    /* The protocol used */
    struct sockaddr_in _address;  /* The IP address on which we are binded */
    int                _socket;   /* File descriptor for the created socket */
    bool               _running;  /* If the receiver in receiving or not */
    pthread_t          _thread;   /* The receiver thread */

} Receiver;

extern Receiver* Receiver_new(char* _addr, u_int16_t _port, char* _proto);
extern void Receiver_delete(Receiver* _self);

extern void  Receiver_start(Receiver* _self);
extern void* Receiver_run(void* _self);
extern void  Receiver_stop(Receiver* _self);

__END_DECLS

#endif