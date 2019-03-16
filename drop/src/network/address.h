// Forward declarations

namespace drop
{
    // Tags

    class IPv4;
    class IPv6;

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
#include <ifaddrs.h>
#include <net/if.h>
#include <iostream>
#include <vector>

// Includes

#include "drop/bytewise/bytewise.h"
#include "drop/data/variant.hpp"

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

        // Bytewise

        template <typename atype> void accept(bytewise :: reader <atype> &) const;
        template <typename atype> void accept(bytewise :: writer <atype> &);

        // Getters

        class ip ip() const;
        class port port() const;

        template <typename type, std :: enable_if_t <std :: is_same <type, IPv4> :: value || std :: is_same <type, IPv6> :: value> * = nullptr> bool is() const;

        // Methods

        address decay() const;

        template <typename... lambdas, std :: enable_if_t <variant <sockaddr_in, sockaddr_in6> :: constraints :: match <false, lambdas...> ()> * = nullptr> void match(lambdas && ...);
        template <typename... lambdas, std :: enable_if_t <variant <sockaddr_in, sockaddr_in6> :: constraints :: match <true, lambdas...> ()> * = nullptr> void match(lambdas && ...) const;

        // Operators

        bool operator == (const address &) const;
        bool operator != (const address &) const;
    };

    class address :: ip
    {
        // Friends

        friend class address;

        // Members

        variant <in_addr, in6_addr> _addr;

    public:

        // Constructors

        ip();
        ip(const char *);
        ip(const address &);

        ip(const in_addr &);
        ip(const in6_addr &);

        // Bytewise

        template <typename atype> void accept(bytewise :: reader <atype> &) const;
        template <typename atype> void accept(bytewise :: writer <atype> &);

        // Getters

        template <typename type, std :: enable_if_t <std :: is_same <type, IPv4> :: value || std :: is_same <type, IPv6> :: value> * = nullptr> bool is() const;

        // Methods

        template <typename... lambdas, std :: enable_if_t <variant <in_addr, in6_addr> :: constraints :: match <false, lambdas...> ()> * = nullptr> void match(lambdas && ...);
        template <typename... lambdas, std :: enable_if_t <variant <in_addr, in6_addr> :: constraints :: match <true, lambdas...> ()> * = nullptr> void match(lambdas && ...) const;

        ip decay() const;

        // Operators

        bool operator == (const ip &) const;
        bool operator != (const ip &) const;

        // Static methods

        template <typename> static ip any();
        static std :: vector <ip> local(const uint32_t & = 0);

    private:

        // Private static methods

        static int version(const char *);
        template <typename atype> static atype pton(const char *);
    };

    class address :: port
    {
        // Friends

        friend class address;

        // Members

        union
        {
            in_port_t port;
            std :: array <uint8_t, 2> representation;
        } _port;

        // Bytewise

        $bytewise(_port.representation);

    public:

        // Constructors

        port();
        port(const uint16_t &);
        port(const address &);

        // Operators

        bool operator == (const port &) const;
        bool operator != (const port &) const;

        // Casting

        explicit operator uint16_t () const;
    };

    // Ostream integration

    std :: ostream & operator << (std :: ostream &, const address &);
    std :: ostream & operator << (std :: ostream &, const class address :: ip &);
    std :: ostream & operator << (std :: ostream &, const class address :: port &);
};

#endif
