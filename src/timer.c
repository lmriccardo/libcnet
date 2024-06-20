#include "utils/timer.h"
#include "sender.h"
#include "receiver.h"

double computeElapsedTime(struct timespec start, struct timespec end)
{
    return NS_PER_SECOND * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
}

struct Timer* Timer_new()
{
    struct Timer *timer = (struct Timer*)malloc(sizeof(struct Timer));
    clock_gettime(CLOCK_REALTIME, &timer->_start);

    timer->_elapsed = 0.0;
    timer->_current = timer->_start;
    timer->_previous = timer->_start;
    timer->_running = false;

    return timer;
}

void Timer_delete(struct Timer* _self)
{
    free(_self);
}

void* Timer_run(void* _self)
{
    while (1)
    {
        if (!((struct Timer*)_self)->_running) break;

        // Update all the values
        clock_gettime(CLOCK_REALTIME, &((struct Timer*)_self)->_current);

    }

    return NULL;
}

void Timer_start(struct Timer* _self)
{
    _self->_running = true;

    pthread_t timer_thread;
    pthread_create(&timer_thread, NULL, Timer_run, _self);
    pthread_detach(timer_thread);
}

double Timer_getDeltaTime(struct Timer* _self)
{

    double eta = computeElapsedTime(_self->_previous, _self->_current);
    _self->_previous = _self->_current;

    return eta / 1e9;
}

double Timer_getElapsedTime(struct Timer* _self)
{

    double elapsed = computeElapsedTime(_self->_start, _self->_current);
    _self->_elapsed = elapsed;

    return elapsed;
}

void Timer_reset(struct Timer* _self)
{
    clock_gettime(CLOCK_REALTIME, &_self->_start);
}

void Timer_resetPrevious(struct Timer* _self)
{
    clock_gettime(CLOCK_REALTIME, &_self->_previous);
}

void Timer_stop(struct Timer* _self)
{
    _self->_running = false;
    sleep(0.75);
}

void Timer_sleep(const double _delay)
{
    double eta;
    struct timespec start;
    struct timespec current;

    clock_gettime(CLOCK_REALTIME, &start);

    do 
    {
        clock_gettime(CLOCK_REALTIME, &current);
        eta = computeElapsedTime(start, current);

    } while (eta < _delay * 1e9);

}