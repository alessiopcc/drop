// Forward declarations

namespace drop
{
    // Tags

    class malformed_address;

    // Classes

    class address;
};

#if !defined(__forward__) && !defined(__src__network__address__h)
#define __src__network__address__h

// Libraries

#include <stddef.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <iostream>

// Includes

#include "bytewise/bytewise.h"
#include "data/variant.hpp"

namespace drop
{
    // Classes

    class address
    {
    public:

        // Nested classes

        class ip;
        class port;

    private:

        // Members

        variant <sockaddr_in, sockaddr_in6> _sockaddr;

    public:

        // Constructors

        address();
        address(const char *, const uint16_t &, const uint32_t & = 0);
        address(const ip &, const port &, const uint32_t & = 0);

        address(const sockaddr_in &);
        address(const sockaddr_in6 &);

        // Getters

        class ip ip() const;
        class port port() const;
    };

    class address :: ip
    {
        // Friends

        friend class address;
        friend std :: ostream & operator << (std :: ostream &, const ip &);

        // Members

        variant <in_addr, in6_addr> _ip;

    public:

        // Constructors

        ip();
        ip(const char *);
        ip(const address &);

    private:

        // Private static methods

        static int version(const char *);
        template <typename atype> static atype pton(const char *);
    };

    class address :: port
    {
        // Friends

        friend class address;
        friend std :: ostream & operator << (std :: ostream &, const port &);

        // Members

        in_port_t _port;

    public:

        // Constructors

        port();
        port(const uint16_t &);
        port(const address &);
    };

    // Ostream integration

    std :: ostream & operator << (std :: ostream &, const address &);
    std :: ostream & operator << (std :: ostream &, const class address :: ip &);
    std :: ostream & operator << (std :: ostream &, const class address :: port &);
};

#endif
