/***********************************************************************************
 ***********************************************************************************
 *****                                                                         *****
 *****   @details Header file                                                  *****
 *****   @name receiver.h                                                      *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)              *****
 *****   @date 1 June 2024 (Mon Jul 01 2024 Last Modification)                 *****
 *****                                                                         *****
 *****   @brief This header file contains the declaration of the Receiver      *****
 *****   object struct and a number of functions to interact with it.          *****
 *****   The receiver is defined is ran as a thread and uses a MessageQueue    *****
 *****   to save all the messages it receives.                                 *****
 *****                                                                         *****
 ***********************************************************************************
 ***********************************************************************************
*/


#ifndef _RECEIVER_H
#define _RECEIVER_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <unistd.h>

#include "ip.h"
#include "utils/timer.h"
#include "utils/net.h"
#include "utils/list.h"
#include "utils/synch.h"
#include "utils/version.h"

#define handle_error(msg, fd) do { perror(msg); shutdown(fd, 2); exit(EXIT_FAILURE); } while (0)

typedef LinkedList MessageQueue;

#define MessageQueue_new(x)        (LinkedList_new(x))
#define MessageQueue_delete(x)     (LinkedList_deletet(x))
#define MessageQueue_push(x, y, z) (LinkedList_pushvt(x, y, z))
#define MessageQueue_pop(x)        (LinkedList_popt(x))
#define MessageQueue_isEmpty(x)    (LinkedList_isEmptyt(x))
#define MessageQueue_getSize(x)    (LinkedList_getSizet(x))

__BEGIN_DECLS


/**
 * Struct representing a Receiver. Essentially, it is an always running thread
 * listening for incoming packets on a defined protocol on a given port.
 * 
 * It has the following fields:
 * - `_proto` is a pointer to a `protoent` structure defining which protocol is used
 * - `_address` is a `sockaddr_in` value containing the IP address on which the receiver is binded
 * - `_socket` is just the file descriptor of the created socket
 * - `_running` a boolean indicating if the receiver is running or not
 * - `_verbose` A flag to enable verbosity
 * - `_timer` A timer shared with the Sender
 * - `_mutex` A simple Mutex to syncrhonize with the sender
 */
typedef struct 
{

   struct protoent*   _proto;   //!< The protocol used
   struct sockaddr_in _address; //!< The IP address on which we are binded
   int                _socket;  //!< File descriptor for the created socket
   bool               _running; //!< If the receiver in receiving or not
   bool               _verbose; //!< Enable verbosity
   struct Timer*      _timer;   //!< An optional Timer
   sem_t*             _mutex;   //!< A semaphore to synchronize with the Sender
   MessageQueue*      _queue;   //!< The message queue, essentially a linked list

} Receiver;

/**
 * A simple struct containing the response received by the receiver
 */
struct Response
{

   char*  _buffer; //!< The buffer containing the received IP Packet
   size_t _size;   //!< The length of the entire buffer
   double _rtt;    //!< Essentially the Round-Trip-Time

};

/**
 * Creates a new Receiver given as input the string containing an IP address,
 * a port and the protocol. This function binds the newly created socket
 * using this input values.
 *
 * The first argument is the interface to which bind the receiver, for example
 * eth0. Given the interface, it takes the associated IP address.
 */
extern Receiver* Receiver_new(
   const char* _interface, const u_int16_t _port, const char* _proto, const bool _verbose)
   __attribute__((nonnull)) __attribute__((returns_nonnull));

/**
 * Free the memory allocated for the input Receiver
 */
extern void Receiver_delete(Receiver* _self) __attribute__((nonnull));

/**
 * Start a new Receiver thread
 */
extern void  Receiver_start(Receiver* _self) __attribute__((nonnull));

/**
 * The function the Receiver thread will always execute. This function is called by
 * `Receiver_start` and passed to the newly created thread. It is an infinite loop
 * that every time waits for a packet to be received and put the received packet, if
 * it is correct, into a MessageQueue. The only way to stop the Receiver thread is
 * by calling the corresponding `Receiver_stop` function. 
 */
extern void* Receiver_run(void* _self) __attribute__((nonnull));

/**
 * Stop the Receiver Thread
 */
extern void  Receiver_stop(Receiver* _self) __attribute__((nonnull));

/**
 * Set a new timer into the receiver
 */
extern void Receiver_setTimer(Receiver* _self, struct Timer* _timer) __attribute__((nonnull));

/**
 * Set the input semaphore into the receiver
 */
extern void Receiver_setSemaphore(Receiver* _self, sem_t* _mutex) __attribute__((nonnull));

/**
 * Free the memory allocated for the response
 */
extern void Response_free(struct Response* _self) __attribute__((nonnull));

__END_DECLS

#endif