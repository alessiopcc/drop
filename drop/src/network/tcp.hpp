#ifndef __src__network__tcp__hpp
#define __src__network__tcp__hpp

// Includes

#include "tcp.h"
#include "connection.h"
#include "listener.h"

namespace drop
{
    // socket

    // Getters

    template <> inline auto tcp :: socket :: get <descriptor> () const
    {
        this->opencheck();
        return this->_descriptor;
    }

    template <> inline auto tcp :: socket :: get <blocking> () const
    {
        return !(this->fcntl() & O_NONBLOCK);
    }

    template <> inline auto tcp :: socket :: get <timeouts :: send> () const
    {
        return interval(this->getsockopt <timeval> (SOL_SOCKET, SO_SNDTIMEO));
    }

    template <> inline auto tcp :: socket :: get <timeouts :: receive> () const
    {
        return interval(this->getsockopt <timeval> (SOL_SOCKET, SO_RCVTIMEO));
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

    template <> inline auto tcp :: socket :: get <reuse> () const
    {
        return this->getsockopt <int> (SOL_SOCKET, SO_REUSEADDR);
    }

    // Setters

    template <> inline void tcp :: socket :: set <blocking, bool> (const bool & value) const
    {
        if(value)
            this->fcntl(this->fcntl() & ~(O_NONBLOCK));
        else
            this->fcntl(this->fcntl() | O_NONBLOCK);
    }

    template <> inline void tcp :: socket :: set <timeouts :: send, interval> (const interval & value) const
    {
        this->setsockopt(SOL_SOCKET, SO_SNDTIMEO, (timeval) value);
    }

    template <> inline void tcp :: socket :: set <timeouts :: receive, interval> (const interval & value) const
    {
        this->setsockopt(SOL_SOCKET, SO_RCVTIMEO, (timeval) value);
    }

    template <> inline void tcp :: socket :: set <reuse> (const bool & value) const
    {
        if(value)
            this->setsockopt(SOL_SOCKET, SO_REUSEADDR, 1);
        else
            this->setsockopt(SOL_SOCKET, SO_REUSEADDR, 0);
    }

    // Private methods

    template <typename type> type tcp :: socket :: getsockopt(const int & level, const int & optname) const
    {
        this->opencheck();

        type value;
        socklen_t size = sizeof(type);

        if(:: getsockopt(this->_descriptor, level, optname, (uint8_t *) &value, &size))
            exception <bad_access, getsockopt_failed> :: raise(this, errno);

        return value;
    }

    template <typename type> void tcp :: socket :: setsockopt(const int & level, const int & optname, const type & value) const
    {
        this->opencheck();

        if(:: setsockopt(this->_descriptor, level, optname, (const uint8_t *) &value, sizeof(type)))
            exception <bad_access, setsockopt_failed> :: raise(this, errno);
    }

    // listener

    // Getters

    template <> inline auto tcp :: listener :: get <timeouts :: accept> ()
    {
        return this->_guard([&]()
        {
            return this->_cache.timeout;
        });
    }

    // Setters

    template <> inline void tcp :: listener :: set <timeouts :: accept> (const interval & timeout)
    {
        this->_guard([&]()
        {
            this->_cache.timeout = timeout;
        });
    }

    // Private methods

    template <bool value> void tcp :: listener :: block()
    {
        if(this->_cache.blocking != value)
        {
            this->_socket.set <blocking> (value);
            this->_cache.blocking = value;
        }
    }

    template <bool blocking> void tcp :: listener :: setup()
    {
        this->_guard([&]()
        {
            if(this->_lock)
                exception <accept_failed, listener_locked> :: raise(this);

            this->block <blocking> ();
            this->_lock = true;
        });
    }
};

#endif
