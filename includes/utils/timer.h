#ifndef _TIMER_H
#define _TIMER_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <time.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include "utils/version.h"

#ifndef __USE_POSIX199309
    #define CLOCK_REALTIME 0
#endif

#define _POSIX_C_SOURCE 200809L

#define NS_PER_SECOND 1000000000

__BEGIN_DECLS

/* Compute the difference between two timespec t2 and t1 and returns the value in seconds */
extern double computeElapsedTime(struct timespec t1, struct timespec t2);

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
    struct timespec _start;     // The time when the Timer is created
    struct timespec _current;   // The current time, updated at each iteration
    struct timespec _previous;  // The time when the `Timer_getTime` function is called
    double          _elapsed;   // The total elapsed time from the start
    bool            _running;   // Is true if the corresponding thread is running
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
extern double Timer_getDeltaTime(struct Timer* _self) __attribute__((nonnull));

/* Returns the elapsed time in seconds. We need to make a function since it access
   concurrent region, hence we need to semaphore it.
*/
extern double Timer_getElapsedTime(struct Timer* _self) __attribute__((nonnull));

/* Reset the start value to the current time */
extern void Timer_reset(struct Timer* _self) __attribute__((nonnull));

/* Reset the previous value with the current time */
extern void Timer_resetPrevious(struct Timer* _self) __attribute__((nonnull));

/* Custom Sleep function which accepts floating delay values */
extern void Timer_sleep(const double _delay) __attribute__((nonnull));

__END_DECLS


#endif