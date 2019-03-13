// Includes

#include "semaphore.h"

namespace drop
{
    // Constructors

    semaphore :: semaphore()
    {
        #ifdef __APPLE__
        this->_semaphore = dispatch_semaphore_create(0);
        #endif

        #ifdef __linux__
        sem_init(&(this->_semaphore), 0, 0);
        #endif
    }

    // Destructor

    semaphore :: ~semaphore()
    {
        #ifdef __APPLE__
        dispatch_release(this->_semaphore);
        #endif

        #ifdef __linux__
        sem_destroy(&(this->_semaphore));
        #endif
    }

    // Methods

    bool semaphore :: wait()
    {
        #ifdef __APPLE__
        return dispatch_semaphore_wait(this->_semaphore, DISPATCH_TIME_FOREVER);
        #endif

        #ifdef __linux__
        return sem_wait(&(this->_semaphore));
        #endif
    }

    bool semaphore :: wait(const timestamp & timeout)
    {
        timestamp offset = now();

        if(offset >= timeout)
            return true;
        else
            return this->wait(timeout - offset);
    }

    bool semaphore :: wait(const interval & timeout)
    {
        #ifdef __APPLE__
        dispatch_time_t time = dispatch_time(DISPATCH_TIME_NOW, (const uint64_t &) timeout * NSEC_PER_USEC);
        return dispatch_semaphore_wait(this->_semaphore, time);
        #endif

        #ifdef __linux__
        timeval target = (timeval) (now() + timeout);

        timespec timespec
        {
            .tv_sec = target.tv_sec,
            .tv_nsec = target.tv_usec * 1000
        };

        return sem_timedwait(&(this->_semaphore), &timespec);
        #endif
    }

    void semaphore :: post()
    {
        #ifdef __APPLE__
        dispatch_semaphore_signal(this->_semaphore);
        #endif

        #ifdef __linux__
        sem_post(&(this->_semaphore));
        #endif
    }
};
