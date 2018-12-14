// Includes

#include "queue.hpp"

namespace drop
{
    // event

    // Getters

    int queue :: event :: descriptor() const
    {
        #ifdef __APPLE__
        return this->ident;
        #endif

        #ifdef __linux__
        return this->data.fd;
        #endif
    }

    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wreturn-type"
    queue :: type queue :: event :: type() const
    {
        #ifdef __APPLE__
        if(this->filter == EVFILT_WRITE)
            return write;
        else if(this->filter == EVFILT_READ)
            return read;
        else
            exception <unknown_event> :: raise(this, this->filter);
        #endif

        #ifdef __linux__
        if(this->events & EPOLLOUT)
            return write;
        else if(this->events & EPOLLIN)
            return read;
        else
            exception <unknown_event> :: raise(this, this->events);
        #endif
    }
    #pragma clang diagnostic pop

    bool queue :: event :: error() const
    {
        #ifdef __APPLE__
        return (this->flags & EV_ERROR);
        #endif

        #ifdef __linux__
        return (this->events & (EPOLLERR | EPOLLHUP));
        #endif
    }

    // queue

    // Constructors

    queue :: queue()
    {
        #ifdef __APPLE__
        this->_descriptor = kqueue();
        #endif

        #ifdef __linux__
        this->_descriptor = epoll_create1(0);
        #endif
    }

    // Destructor

    queue :: ~queue()
    {
        close(this->_descriptor);
    }

    // Methods

    void queue :: add(const type & filter, const int & descriptor)
    {
        #ifdef __APPLE__
        this->kevent(filter, descriptor, EV_ADD);
        #endif

        #ifdef __linux__
        this->epoll_ctl(filter, descriptor, EPOLL_CTL_ADD);
        #endif
    }

    void queue :: remove(const type & filter, const int & descriptor)
    {
        #ifdef __APPLE__
        this->kevent(filter, descriptor, EV_DELETE);
        #endif

        #ifdef __linux__
        this->epoll_ctl(filter, descriptor, EPOLL_CTL_DEL);
        #endif
    }

    // Private methods

    #ifdef __APPLE__
    void queue :: kevent(const type & filter, const int & descriptor, const uint16_t & operation)
    {
        struct kevent event
        {
            .ident = (uintptr_t) descriptor,
            .flags = operation,
            .filter = (int16_t) ((filter == write) ? EVFILT_WRITE : EVFILT_READ),
            .fflags = 0,
            .data = 0,
            .udata = nullptr
        };

        if(:: kevent(this->_descriptor, &event, 1, 0, 0, 0) < 0)
            exception <kevent_failed> :: raise(this, errno);
    }
    #endif

    #ifdef __linux__
    void queue :: epoll_ctl(const type & filter, const int & descriptor, const int & operation)
    {
        struct epoll_event event
        {
            .data.fd = descriptor,
            .events = ((filter == write) ? EPOLLOUT : EPOLLIN)
        };

        if(:: epoll_ctl(this->_descriptor, operation, descriptor, &event) < 0)
            exception <epoll_ctl_failed> :: raise(this, errno);
    }
    #endif
};
