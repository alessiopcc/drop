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

    // Methods

    address address :: decay() const
    {
        return address(this->ip().decay(), this->port());
    }

    // Operators

    bool address :: operator == (const address & rho) const
    {
        bool equal = false;

        this->decay()._sockaddr.match([&]()
        {
            rho.decay()._sockaddr.match([&]()
            {
                equal = true;
            });
        }, [&](const sockaddr_in & sockaddr)
        {
            rho.decay()._sockaddr.match([&](const sockaddr_in & rhosockaddr)
            {
                equal = (sockaddr.sin_addr.s_addr == rhosockaddr.sin_addr.s_addr) && (sockaddr.sin_port == rhosockaddr.sin_port);
            });
        }, [&](const sockaddr_in6 & sockaddr)
        {
            rho.decay()._sockaddr.match([&](const sockaddr_in6 & rhosockaddr)
            {
                equal = !(memcmp(sockaddr.sin6_addr.s6_addr, rhosockaddr.sin6_addr.s6_addr, 16)) && (sockaddr.sin6_port == rhosockaddr.sin6_port);
            });
        });

        return equal;
    }

    bool address :: operator != (const address & rho) const
    {
        return !((*this) == rho);
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

    address :: ip :: ip(const in_addr & addr) : _addr(addr)
    {
    }

    address :: ip :: ip(const in6_addr & addr) : _addr(addr)
    {
    }

    // Methods

    class address :: ip address :: ip :: decay() const
    {
        ip decay;

        this->match([&](const in_addr & inaddr)
        {
            decay = inaddr;
        }, [&](const in6_addr & inaddr)
        {
            static std :: array <uint8_t, 12> mask = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xff, 0xff};
            if(memcmp(inaddr.s6_addr, mask.data(), mask.size()))
                decay = inaddr;
            else
            {
                in_addr crop;
                memcpy(&(crop.s_addr), inaddr.s6_addr + mask.size(), sizeof(in_addr));
                decay = crop;
            }
        });

        return decay;
    }

    // Operators

    bool address :: ip :: operator == (const ip & rho) const
    {
        bool equal = false;

        this->decay()._addr.match([&]()
        {
            rho.decay()._addr.match([&]()
            {
                equal = true;
            });
        }, [&](const in_addr & addr)
        {
            rho.decay()._addr.match([&](const in_addr & rhoaddr)
            {
                equal = (addr.s_addr == rhoaddr.s_addr);
            });
        }, [&](const in6_addr & addr)
        {
            rho.decay()._addr.match([&](const in6_addr & rhoaddr)
            {
                equal = !(memcmp(addr.s6_addr, rhoaddr.s6_addr, 16));
            });
        });

        return equal;
    }

    bool address :: ip :: operator != (const ip & rho) const
    {
        return !((*this) == rho);
    }

    // Static methods

    template <> class address :: ip address :: ip :: any <IPv4> ()
    {
        return in_addr{.s_addr = INADDR_ANY};
    }

    template <> class address :: ip address :: ip :: any <IPv6> ()
    {
        return in6addr_any;
    }

    std :: vector <class address :: ip> address :: ip :: local(const uint32_t & scope)
    {
        std :: vector <ip> addresses;

        struct ifaddrs * interfaces;
        getifaddrs(&interfaces);

        for(struct ifaddrs * interface = interfaces; interface; interface = interface->ifa_next)
            if((interface->ifa_flags & IFF_UP) && !(interface->ifa_flags & IFF_LOOPBACK) && !(interface->ifa_flags & IFF_POINTOPOINT))
            {
                if(interface->ifa_addr->sa_family == AF_INET)
                    addresses.push_back(((sockaddr_in *) (interface->ifa_addr))->sin_addr);
                else if((interface->ifa_addr->sa_family == AF_INET6) && ((sockaddr_in6 *) (interface->ifa_addr))->sin6_scope_id == scope)
                    addresses.push_back(((sockaddr_in6 *) (interface->ifa_addr))->sin6_addr);
            }

        freeifaddrs(interfaces);

        return addresses;
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

    // Operators

    bool address :: port :: operator == (const port & rho) const
    {
        return (this->_port == rho._port);
    }

    bool address :: port :: operator != (const port & rho) const
    {
        return (this->_port != rho._port);
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
