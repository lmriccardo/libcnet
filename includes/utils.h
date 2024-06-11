#ifndef _UTILS_H
#define _UTILS_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

__BEGIN_DECLS

/* Returns the IP address of the input host */
extern char* getHostnameIP(const char* _hostname) 
    __attribute__((nonnull)) __attribute__((returns_nonnull));

/* Returns the IP address of the given input interface */
extern void getInterfaceIp(const char* _interface, char *_addr)
    __attribute__((nonnull));

/* Converts the input address number into a string. The second argument is used
   to specify whether the input address number is in LE or BE format.

   The returns of this function is dynamically allocated, hence it must be freed.
*/
extern char* addressNumberToString(u_int32_t _addr, const bool _be) 
    __attribute__((returns_nonnull));

/* Converts the input address number into a string. The third argument is used
   to specify whether the input address number is in LE or BE format. The result
   of the operation will be saved into the _out input buffer.
*/
extern void addressNumberToString_s(u_int32_t _addr, char *_out, const bool _be) 
    __attribute__((nonnull));

/* Struct representing a simple timer. It can be used to time the elapsed time
   between the message being sent and the response being received by the receiver.
   The timer starts by calling the corresponding `Timer_start` function. 
   
   The means of the fields is the following:
   - `_start` the time when the Timer is created
   - `_elapsed` the elapsed time between start and current
   - `_current` the current time everytime updated
   - `_previous` the time when the `Timer_getTime` function is called
*/
struct Timer 
{
    clock_t _start;
    clock_t _elapsed;
    clock_t _current;
    clock_t _previous;
    bool    _running;
    sem_t   _mutex;
};

/* Create and returns new Timer. This function does dynamic allocation
   which means that, it must be freed using the corresponding 
   Timer_delete function. 
*/
extern struct Timer* Timer_new() __attribute__((returns_nonnull));

/* Free the memory allocated for the input Timer */
extern void Timer_delete(struct Timer* _self) __attribute__((nonnull));

/* Start the timer starting the corresponding thread */
extern void Timer_start(struct Timer* _self) __attribute__((nonnull));

/* Iteratively update the timer */
extern void* Timer_run(void* _self) __attribute__((nonnull));

/* Stop the timer thread */
extern void Timer_stop(struct Timer* _self) __attribute__((nonnull));

/* Returns the difference between the current time and the previous time.
   Every time this function is called, computes the delta, updates the previous 
   time with the value of the current time and returns the delta time.
*/
extern clock_t Timer_getDeltaTime(struct Timer* _self) __attribute__((nonnull));

extern void __semaphore_init(
    sem_t* _sem, int _phsared, unsigned int _value, const char* _fname) __attribute__((nonnull));

extern void __semaphore_wait(sem_t* _sem, const char* _fname) __attribute__((nonnull));
extern void __semaphore_post(sem_t* _sem, const char* _fname) __attribute__((nonnull));

__END_DECLS

#endif