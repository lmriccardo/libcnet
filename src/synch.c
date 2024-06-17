#include "utils/synch.h"
#include "sender.h"
#include "receiver.h"
#include "utils/timer.h"


void semaphore_init(sem_t* _sem, int _phsared, unsigned int _value, const char* _fname)
{
    int ret = sem_init(_sem, _phsared, _value);
    if (ret != 0)
    {
        printf("[%s] call to sem_init failed\n", _fname); 
        exit(EXIT_FAILURE);
    }
}

void semaphore_wait(sem_t* _sem, const char* _fname)
{
    int ret = sem_wait(_sem);
    if (ret != 0)
    {
        printf("[%s] call to sem_wait failed\n", _fname); 
        exit(EXIT_FAILURE);
    }
}

void semaphore_post(sem_t* _sem, const char* _fname)
{
    int ret = sem_post(_sem);
    if (ret != 0)
    {
        printf("[%s] call to sem_post failed\n", _fname); 
        exit(EXIT_FAILURE);
    }
}

void synchronizeRTT(void* _sender, void* _recv, void* _timer)
{
    Sender_setTimer((Sender*)_sender, (struct Timer*)_timer);
    Receiver_setTimer((Receiver*)_recv, (struct Timer*)_timer);
}

void syncrhonizeSendReceive(void* _sender, void* _recv)
{
    semaphore_init(&((Sender*) _sender)->_mutex, 0, 1, "synchronizeSendReceive");
    ((Sender*)_sender)->_synch = true;

    Receiver_setSemaphore((Receiver *)_recv, &((Sender*) _sender)->_mutex);
}