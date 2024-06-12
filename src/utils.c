#include "utils.h"
#include "sender.h"
#include "receiver.h"

char* getHostnameIP(const char* _hostname)
{
    struct hostent *hostentry = gethostbyname(_hostname);
    if (hostentry == NULL)
    {
        fprintf(stderr, "[getHostName_s] GetHostByName call failed.\n");
        exit(EXIT_FAILURE);
    }

    char *ip = inet_ntoa(*((struct in_addr*) hostentry->h_addr_list[0]));

    return ip;
}

char* addressNumberToString(u_int32_t _addr, const bool _be)
{
    // If it is in Little-Endian, then convert it to Big-Endian
    if (!_be) _addr = htonl(_addr);

    char *addr_str = (char *)malloc(INET_ADDRSTRLEN * sizeof(char));
    inet_ntop(AF_INET, &_addr, addr_str, INET_ADDRSTRLEN);

    return addr_str;
}

void addressNumberToString_s(u_int32_t _addr, char *_out, const bool _be)
{
    // If it is in Little-Endian, then convert it to Big-Endian
    if (!_be) _addr = htonl(_addr);
    inet_ntop(AF_INET, &_addr, _out, INET_ADDRSTRLEN);
}

void getInterfaceIp(const char* _interface, char* _addr)
{
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , _interface , IFNAMSIZ - 1);
    ioctl(sockfd, SIOCGIFADDR, &ifr);
    close(sockfd);

    u_int32_t _addr_i = ( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr.s_addr;
    inet_ntop(AF_INET, &_addr_i, _addr, INET_ADDRSTRLEN);
}

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

    __semaphore_init(&timer->_mutex, 0, 1, "Timer_new");

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
        
        // Wait for the semphore
        // __semaphore_wait(&((struct Timer*)_self)->_mutex, "Timer_run");

        if (!((struct Timer*)_self)->_running) break;

        // Update all the values
        clock_gettime(CLOCK_REALTIME, &((struct Timer*)_self)->_current);

        // Release the semaphore
        // __semaphore_post(&((struct Timer*)_self)->_mutex, "Timer_run");

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
    // __semaphore_wait(&_self->_mutex, "Timer_getDeltaTime");

    double eta = computeElapsedTime(_self->_previous, _self->_current);
    _self->_previous = _self->_current;

    // __semaphore_post(&_self->_mutex, "Timer_getDeltaTime");

    return eta / 1e9;
}

double Timer_getElapsedTime(struct Timer* _self)
{
    // __semaphore_wait(&_self->_mutex, "Timer_getElapsedTime");

    double elapsed = computeElapsedTime(_self->_start, _self->_current);
    _self->_elapsed = elapsed;

    // __semaphore_post(&_self->_mutex, "Timer_getElapsedTime");

    return elapsed;
}

void Timer_reset(struct Timer* _self)
{
    // __semaphore_wait(&_self->_mutex, "Timer_reset");
    clock_gettime(CLOCK_REALTIME, &_self->_start);
    // __semaphore_post(&_self->_mutex, "Timer_reset");
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

void __semaphore_init(sem_t* _sem, int _phsared, unsigned int _value, const char* _fname)
{
    int ret = sem_init(_sem, _phsared, _value);
    if (ret != 0)
    {
        printf("[%s] call to sem_init failed\n", _fname); 
        exit(EXIT_FAILURE);
    }
}

void __semaphore_wait(sem_t* _sem, const char* _fname)
{
    int ret = sem_wait(_sem);
    if (ret != 0)
    {
        printf("[%s] call to sem_wait failed\n", _fname); 
        exit(EXIT_FAILURE);
    }
}

void __semaphore_post(sem_t* _sem, const char* _fname)
{
    int ret = sem_post(_sem);
    if (ret != 0)
    {
        printf("[%s] call to sem_post failed\n", _fname); 
        exit(EXIT_FAILURE);
    }
}

void synchronizeRTT(void* _sender, void* _recv, struct Timer* _timer)
{
    RawSender_setTimer((RawSender*)_sender, _timer);
    Receiver_setTimer((Receiver*)_recv, _timer);
}