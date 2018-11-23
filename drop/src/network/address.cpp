// Includes

#include "address.hpp"

namespace drop
{
    // Forward declarations

    template <> in_addr address :: ip :: pton <in_addr> (const char *);
    template <> in6_addr address :: ip :: pton <in6_addr> (const char *);

    // address

    // Constructors

    address :: address()
    {
    }

    address :: address(const char * ip, const uint16_t & port, const uint32_t & scope) : address((class ip){ip}, (class port){port}, scope)
    {
    }

    address :: address(const class ip & ip, const class port & port, const uint32_t & scope)
    {
        ip._addr.match([&](const in_addr & ip)
        {
            sockaddr_in sockaddr;
            sockaddr.sin_family = AF_INET;
            sockaddr.sin_port = port._port;
            sockaddr.sin_addr = ip;
            memset(sockaddr.sin_zero, '\0', sizeof(sockaddr.sin_zero));

            this->_sockaddr = sockaddr;
        }, [&](const in6_addr & ip)
        {
            sockaddr_in6 sockaddr;
            sockaddr.sin6_family = AF_INET6;
            sockaddr.sin6_port = port._port;
            sockaddr.sin6_flowinfo = 0;
            sockaddr.sin6_addr = ip;
            sockaddr.sin6_scope_id = scope;

            this->_sockaddr = sockaddr;
        });
    }

    address :: address(const sockaddr_in & sockaddr) : _sockaddr(sockaddr)
    {
    }

    address :: address(const sockaddr_in6 & sockaddr) : _sockaddr(sockaddr)
    {
    }

    // Getters

    class address :: ip address :: ip() const
    {
        return (class ip){*this};
    }

    class address :: port address :: port() const
    {
        return (class port){*this};
    }

    // ip

    // Constructors

    address :: ip :: ip()
    {
    }

    address :: ip :: ip(const char * presentation)
    {
        if(version(presentation) == AF_INET)
            this->_addr = pton <in_addr> (presentation);
        else
            this->_addr = pton <in6_addr> (presentation);
    }

    address :: ip :: ip(const address & address)
    {
        address._sockaddr.match([&](const sockaddr_in & sockaddr)
        {
            this->_addr = sockaddr.sin_addr;
        }, [&](const sockaddr_in6 & sockaddr)
        {
            this->_addr = sockaddr.sin6_addr;
        });
    }

    // Private static methods

    int address :: ip :: version(const char * presentation)
    {
        for(const char * cursor = presentation; *cursor; cursor++)
            if((*cursor) == ':')
                return AF_INET6;

        return AF_INET;
    }

    template <> in_addr address :: ip :: pton <in_addr> (const char * presentation)
    {
        in_addr ip;

        if(inet_pton(AF_INET, presentation, &(ip)) <= 0)
            exception <malformed_address> :: raise();

        return ip;
    }

    template <> in6_addr address :: ip :: pton <in6_addr> (const char * presentation)
    {
        in6_addr ip;

        if(inet_pton(AF_INET6, presentation, &(ip)) <= 0)
            exception <malformed_address> :: raise();

        return ip;
    }

    // port

    // Constructors

    address :: port :: port()
    {
    }

    address :: port :: port(const uint16_t & port) : _port(htons(port))
    {
    }

    address :: port :: port(const address & address)
    {
        address._sockaddr.match([&](const sockaddr_in & sockaddr)
        {
            this->_port = sockaddr.sin_port;
        }, [&](const sockaddr_in6 & sockaddr)
        {
            this->_port = sockaddr.sin6_port;
        });
    }

    // Casting

    address :: port :: operator uint16_t () const
    {
        return ntohs(this->_port);
    }

    // Ostream integration

    std :: ostream & operator << (std :: ostream & out, const address & address)
    {
        char presentation[INET6_ADDRSTRLEN];

        address.match([&](const sockaddr_in & sockaddr)
        {
            inet_ntop(AF_INET, &(sockaddr.sin_addr), presentation, INET6_ADDRSTRLEN);
            out << "(" << presentation << ":" << ntohs(sockaddr.sin_port) << ")";
        }, [&](const sockaddr_in6 & sockaddr)
        {
            inet_ntop(AF_INET6, &(sockaddr.sin6_addr), presentation, INET6_ADDRSTRLEN);
            out << "([" << presentation << "]:" << ntohs(sockaddr.sin6_port) << ")";
        });

        return out;
    }

    std :: ostream & operator << (std :: ostream & out, const class address :: ip & ip)
    {
        char presentation[INET6_ADDRSTRLEN];

        ip.match([&](const in_addr & ip)
        {
            inet_ntop(AF_INET, &ip, presentation, INET6_ADDRSTRLEN);
        }, [&](const in6_addr & ip)
        {
            inet_ntop(AF_INET6, &ip, presentation, INET6_ADDRSTRLEN);
        });

        return (out << "(" << presentation << ")");
    }

    std :: ostream & operator << (std :: ostream & out, const class address :: port & port)
    {
        return out << ((uint16_t) port);
    }
};
