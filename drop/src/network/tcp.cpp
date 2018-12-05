// Includes

#include "tcp.hpp"
#include "exception/exception.hpp"

namespace drop
{
    // socket

    // Constructors

    tcp :: socket :: socket() : _descriptor(:: socket(PF_INET, SOCK_STREAM, 0)) // (REMOVE ME) _descriptor(-1)
    {
    }

    // Private constructors

    tcp :: socket :: socket(const int & descriptor) : _descriptor(descriptor)
    {
    }

    // Private methods

    void tcp :: socket :: opencheck() const
    {
        if(this->_descriptor < 0)
            exception <bad_access, socket_closed> :: raise(this);
    }

    int tcp :: socket :: fcntl() const
    {
        int flags = :: fcntl(this->_descriptor, F_GETFL);

        if(flags < 0)
            exception <bad_access, fcntl_failed> :: raise(this);

        return flags;
    }

    void tcp :: socket :: fcntl(const int & flags)
    {
        if(:: fcntl(this->_descriptor, F_SETFL, flags))
            exception <bad_access, fcntl_failed> :: raise(this);
    }
};
