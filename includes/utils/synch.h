/****************************************************************************************
 ****************************************************************************************
 *****                                                                              *****
 *****   @details Header file                                                       *****
 *****   @name synch.h                                                              *****
 *****   @author Riccardo La Marca (riccardo.lamarca98@gmail.com)                   *****
 *****   @date 1 June 2024 (Mon Jul 01 2024 Last Modification)                      *****
 *****                                                                              *****
 *****   @brief This file contains declaration of functions used to synchronize     *****
 *****   a Sender and Receiver, between each other or a common Timer object.        *****
 *****                                                                              *****
 ****************************************************************************************
 ****************************************************************************************
*/

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