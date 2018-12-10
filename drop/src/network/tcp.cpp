// Includes

#include "tcp.hpp"
#include "exception/exception.hpp"

namespace drop
{
    // socket

    // Constructors

    tcp :: socket :: socket() : _descriptor(-1)
    {
    }

    // Private constructors

    tcp :: socket :: socket(const int & descriptor) : _descriptor(descriptor)
    {
    }

    // Getters

    address tcp :: socket :: remote() const
    {
        sockaddr_storage remote;
        socklen_t socklen = sizeof(sockaddr_storage);

        if(getpeername(this->_descriptor, (struct sockaddr *) &remote, &socklen))
            exception <bad_access, getpeername_failed> :: raise(this, errno);

        if(((struct sockaddr *) &remote)->sa_family == AF_INET)
            return address(*((struct sockaddr_in *) &remote));
        else
            return address(*((struct sockaddr_in6 *) &remote)).decay();
    }

    // Methods

    void tcp :: socket :: bind(const class address :: port & port)
    {
        this->bind(address(address :: ip :: any <IPv6> (), port), false);
    }

    void tcp :: socket :: bind(const address & address)
    {
        this->bind(address, true);
    }

    void tcp :: socket :: listen()
    {
        this->opencheck();

        if(:: listen(this->_descriptor, settings :: listen :: slots))
            exception <listen_failed> :: raise(this, errno);
    }

    tcp :: socket tcp :: socket :: accept()
    {
        this->opencheck();

        sockaddr_storage remote;
        socklen_t socklen = sizeof(sockaddr_storage);

        int descriptor = :: accept(this->_descriptor, (struct sockaddr *) &remote, &socklen);

        if(descriptor < 0)
            exception <accept_failed> :: raise(this, errno);

        return socket(descriptor);
    }

    void tcp :: socket :: connect(const address & remote)
    {
        int status;

        remote.match([&](const sockaddr_in & sockaddr)
        {
            if(this->_descriptor < 0)
                this->_descriptor = :: socket(PF_INET, SOCK_STREAM, 0);

            status = :: connect(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in));
        }, [&](const sockaddr_in6 & sockaddr)
        {
            if(this->_descriptor < 0)
                this->_descriptor = :: socket(PF_INET6, SOCK_STREAM, 0);

            status = :: connect(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in6));
        });

        if(status && (errno != EINPROGRESS))
            exception <connect_failed> :: raise(this, errno);
    }

    // Private methods

    void tcp :: socket :: opencheck() const
    {
        if(this->_descriptor < 0)
            exception <bad_access, socket_closed> :: raise(this);
    }

    void tcp :: socket :: closedcheck() const
    {
        if(this->_descriptor >= 0)
            exception <bad_access, socket_open> :: raise(this);
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

    void tcp :: socket :: bind(const address & address, const bool & ipv6only)
    {
        this->closedcheck();

        address.match([&](const sockaddr_in & sockaddr)
        {
            this->_descriptor = :: socket(PF_INET, SOCK_STREAM, 0);

            if(:: bind(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in)))
                exception <bind_failed> :: raise(this, errno);
        }, [&](const sockaddr_in6 & sockaddr)
        {
            this->_descriptor = :: socket(PF_INET6, SOCK_STREAM, 0);
            this->setsockopt(IPPROTO_IPV6, IPV6_V6ONLY, (int) ipv6only);

            if(:: bind(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in6)))
                exception <bind_failed> :: raise(this, errno);
        });
    }
};
