#ifndef __src__network__queue__hpp
#define __src__network__queue__hpp

// Includes

#include "queue.h"
#include "exception/exception.hpp"

namespace drop
{
    // Constraints

    template <typename lambda> constexpr bool queue :: constraints :: callback()
    {
        return $expression($type(lambda)($type(const event &)));
    }

    // Methods

    template <typename lambda, std :: enable_if_t <queue :: constraints :: callback <lambda> ()> *> void queue :: select(lambda && callback, const interval & timeout)
    {
        int count;

        #ifdef __APPLE__
        if((const uint64_t &) timeout)
        {
            struct timespec ktimeout
            {
                .tv_sec = (__darwin_time_t) (((const uint64_t &) timeout) / 1000000),
                .tv_nsec = (__darwin_time_t) (((const uint64_t &) timeout) % 1000000) * 1000
            };

            count = :: kevent(this->_descriptor, 0, 0, this->_events.data(), (int) settings :: buffer, &ktimeout);
        }
        else
            count = :: kevent(this->_descriptor, 0, 0, this->_events.data(), (int) settings :: buffer, nullptr);

        if(count < 0)
            exception <kevent_failed> :: raise(this, errno);
        #endif

        #ifdef __linux__
        count = epoll_wait(this->_descriptor, this->_events.data(), (int) settings :: buffer, (int) (((const uint64_t &) timeout) / 1000));

        if(count < 0)
            exception <epoll_wait_failed> :: raise(this, errno);
        #endif

        for(size_t index = 0; index < count; index++)
        {
            if(this->_events[index].descriptor() == this->_wakepipe.read)
            {
                char buffer;
                while(:: read(this->_wakepipe.read, &buffer, 1) >= 0);
                continue;
            }

            callback(this->_events[index]);
        }
    }
};

#endif
