#ifndef __src__network__tcp__hpp
#define __src__network__tcp__hpp

// Includes

#include "tcp.h"

namespace drop
{
    // Getters

    template <> inline auto tcp :: socket :: get <blocking> () const
    {
        this->opencheck();
        return !(this->fcntl() & O_NONBLOCK);
    }

    template <> inline auto tcp :: socket :: get <timeouts :: send> () const
    {
        this->opencheck();

        auto timeval = this->getsockopt <struct timeval> (SOL_SOCKET, SO_SNDTIMEO);
        return interval(timeval.tv_sec * 1000000 + timeval.tv_usec);
    }

    template <> inline auto tcp :: socket :: get <timeouts :: receive> () const
    {
        this->opencheck();

        auto timeval = this->getsockopt <struct timeval> (SOL_SOCKET, SO_RCVTIMEO);
        return interval(timeval.tv_sec * 1000000 + timeval.tv_usec);
    }

    template <> inline auto tcp :: socket :: get <buffers :: send :: size> () const
    {
        return this->getsockopt <int> (SOL_SOCKET, SO_SNDBUF);
    }

    template <> inline auto tcp :: socket :: get <buffers :: send :: pending> () const
    {
        return this->ioctl(TIOCOUTQ);
    }

    template <> inline auto tcp :: socket :: get <buffers :: send :: available> () const
    {
        return this->get <buffers :: send :: size> () - this->get <buffers :: send :: pending> ();
    }

    template <> inline auto tcp :: socket :: get <buffers :: receive :: size> () const
    {
        return this->getsockopt <int> (SOL_SOCKET, SO_RCVBUF);
    }

    template <> inline auto tcp :: socket :: get <buffers :: receive :: available> () const
    {
        return this->ioctl(FIONREAD);
    }

    // Setters

    template <> inline void tcp :: socket :: set <blocking, bool> (const bool & value)
    {
        this->opencheck();

        if(value)
            this->fcntl(this->fcntl() & ~(O_NONBLOCK));
        else
            this->fcntl(this->fcntl() | O_NONBLOCK);
    }

    template <> inline void tcp :: socket :: set <timeouts :: send, interval> (const interval & value)
    {
        this->opencheck();

        struct timeval timeval;
        timeval.tv_sec = ((const uint64_t &) value) / 1000000;
        timeval.tv_usec = ((const uint64_t &) value) % 1000000;

        this->setsockopt(SOL_SOCKET, SO_SNDTIMEO, timeval);
    }

    template <> inline void tcp :: socket :: set <timeouts :: receive, interval> (const interval & value)
    {
        this->opencheck();

        struct timeval timeval;
        timeval.tv_sec = ((const uint64_t &) value) / 1000000;
        timeval.tv_usec = ((const uint64_t &) value) % 1000000;

        this->setsockopt(SOL_SOCKET, SO_RCVTIMEO, timeval);
    }

    // Private methods

    template <typename type> type tcp :: socket :: getsockopt(const int & level, const int & optname) const
    {
        type value;
        socklen_t size = sizeof(type);

        if(:: getsockopt(this->_descriptor, level, optname, (uint8_t *) &value, &size))
            exception <bad_access, getsockopt_failed> :: raise(this, errno);

        return value;
    }

    template <typename type> void tcp :: socket :: setsockopt(const int & level, const int & optname, const type & value)
    {
        if(:: setsockopt(this->_descriptor, level, optname, (const uint8_t *) &value, sizeof(type)))
            exception <bad_access, setsockopt_failed> :: raise(this, errno);
    }
};

#endif
