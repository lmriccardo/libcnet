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

/* Struct representing a Receiver. Essentially, it is an always running thread
   listening for incoming packets on a defined protocol on a given port.
   It has the following fields:
   - `_proto` is a pointer to a `protoent` structure defining which protocol is used
   - `_address` is a `sockaddr_in` value containing the IP address on which the receiver is binded
   - `_socket` is just the file descriptor of the created socket
   - `_running` a boolean indicating if the receiver is running or not
   - `__process_fn` a function that process a received packet 
*/
typedef struct 
{

    struct protoent*   _proto;              /* The protocol used */
    struct sockaddr_in _address;            /* The IP address on which we are binded */
    int                _socket;             /* File descriptor for the created socket */
    bool               _running;            /* If the receiver in receiving or not */
    void *(*__process_fn) (char *, size_t); /* Function to process the response */

} Receiver;

/* Creates a new Receiver given as input the string containing an IP address,
   a port and the protocol. This function binds the newly created socket
   using this input values.
*/
extern Receiver* Receiver_new(const char* _addr, const u_int16_t _port, const char* _proto) 
   __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Free the memory allocated for the input Receiver */
extern void Receiver_delete(Receiver* _self) __attribute__((nonnull));

/* Start a new Receiver thread */
extern void  Receiver_start(Receiver* _self, void *(*__process_fn) (char *, size_t)) __attribute__((nonnull));

/* The function the Receiver thread will always execute. This function is called by
   `Receiver_start` and passed to the newly created thread. It jobs is to receiving
   incoming packets and process them using the process routine defined by the user. 
*/
extern void* Receiver_run(void* _self) __attribute__((nonnull));

/* Stop the Receiver Thread */
extern void  Receiver_stop(Receiver* _self) __attribute__((nonnull));

/* Just a template process function. This function does nothing and should be
   replaced by a user defined function. */
extern void* __process(char* _buff, size_t _len) __attribute__((nonnull)) __attribute__((deprecated));

__END_DECLS

#endif