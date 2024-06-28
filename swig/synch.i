%module synch

%{
#include "utils/synch.h"
%}

extern void synchronizeRTT(void* _sender, void* _recv, void* _timer);
extern void syncrhonizeSendReceive(void* _sender, void* _recv);
extern void semaphore_init(sem_t* _sem, int _phsared, unsigned int _value, const char* _fname);
extern void semaphore_wait(sem_t* _sem, const char* _fname);
extern void semaphore_post(sem_t* _sem, const char* _fname);