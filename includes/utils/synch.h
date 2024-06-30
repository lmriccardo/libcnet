#ifndef _SYNCH_H
#define _SYNCH_H

#include <sys/cdefs.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/version.h"


__BEGIN_DECLS

/**
 * Synchronize a sender and a receiver with the same timer
 */
extern void synchronizeRTT(void* _sender, void* _recv, void* _timer) __attribute__((nonnull));

/**
 * Synchronize the sender and the receiver setting a semaphore
 */
extern void syncrhonizeSendReceive(void* _sender, void* _recv) __attribute__((nonnull));

/**
 * Initialize a semaphore
 */
extern void semaphore_init(sem_t* _sem, int _phsared, unsigned int _value, const char* _fname) __attribute__((nonnull));

/**
 * Await for the semaphore to be unlocked 
 */
extern void semaphore_wait(sem_t* _sem, const char* _fname) __attribute__((nonnull));

/**
 * Signal all the other processes to access critical resources
 */
extern void semaphore_post(sem_t* _sem, const char* _fname) __attribute__((nonnull));

__END_DECLS

#endif