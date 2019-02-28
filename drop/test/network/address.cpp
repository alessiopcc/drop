#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/address.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;

    // Methods

    bool checkip(const char * expected, const class address :: ip & ip)
    {
        char presentation[INET6_ADDRSTRLEN];

        ip.match([&](const in_addr & ip)
        {
            inet_ntop(AF_INET, &ip, presentation, INET6_ADDRSTRLEN);
        }, [&](const in6_addr & ip)
        {
            inet_ntop(AF_INET6, &ip, presentation, INET6_ADDRSTRLEN);
        });

        return std :: strcmp(expected, presentation);
    }

    // Tests

    $test("address/address", []
    {
        auto myaddress = address("111.11.1.11", 1111);
        if(checkip("111.11.1.11", myaddress.ip()))
            throw "`address` constructor does not assign the correct ip.";
        if((uint16_t)myaddress.port() != 1111)
            throw "`address` constructor does not assign the correct port number.";

        auto ip = (class address :: ip){"127.0.0.1"};
        auto port = (class address :: port){1234};
        myaddress = address(ip, port);
        if(checkip("127.0.0.1", myaddress.ip()))
            throw "`address` constructor does not assign the correct ip number when a `ip` is passed as argument.";
        if((uint16_t)myaddress.port() != 1234)
            throw "`address` constructor does not assign the correct port number when a `port` is passed as argument.";

        auto myotheraddress = address("2222::2", 2222);
            if(checkip("2222::2", myotheraddress.ip()))
                throw "`address` constructor does not assign the correct ip number when a IPv6 address is specified.";

        if(checkip("2222::2", address("2222:0000:0000:0000:0000:0000:0000:0002", 2222).ip()))
            throw "IPv6 addreses are not abbreviated to 'short representation'.";

        if(myaddress.is <IPv6>() || !myaddress.is <IPv4> ())
            throw "`is` does not return the correct value when called on an IPv4 address.";
        if(myotheraddress.is <IPv4>() || !myotheraddress.is <IPv6> ())
            throw "`is` does not return the correct value when called on an IPv6 address.";

        if(checkip("127.0.0.1", myaddress.decay()))
            throw "`decay` should not modify an IPv4 ip address.";
        if(checkip("2222::2", myotheraddress.decay()))
            throw "`decay` should not modify and IPv6 which is not a representation of an IPv4 address.";
        if(checkip("192.0.2.128", address("::ffff:192.0.2.128", 1234).decay()))
            throw "`decay` should extract the corresponding IPv4 from an IPv6 representing an IPv4 address.";

        lastop = 'X';
        myaddress.match([&](const sockaddr_in & sockaddr)
        {
            lastop = '4';
        }, [&](const sockaddr_in6 & sockaddr)
        {
            lastop = '6';
        }, [&]()
        {
            lastop = '?';
        });
        if(lastop != '4')
            throw "`match` does not execute the correct match case when called on an IPv4 `address`.";

        lastop = 'X';
        myotheraddress.match([&](const sockaddr_in & sockaddr)
        {
            lastop = '4';
        }, [&](const sockaddr_in6 & sockaddr)
        {
            lastop = '6';
        }, [&]()
        {
            lastop = '?';
        });
        if(lastop != '6')
            throw "`match` does not execute the correct match case when called on an IPv6 `address`.";

        lastop = 'X';
        address().match([&](const sockaddr_in & sockaddr)
        {
            lastop = '4';
        }, [&](const sockaddr_in6 & sockaddr)
        {
            lastop = '4';
        }, [&]()
        {
            lastop = '?';
        });
        if(lastop != '?')
            throw "`match` does not execute the correct match case when called on an uninitialized `address`.";
    });

    $test("address/ip", []
    {

        auto myip = (class address :: ip){"111.111.111.111"};
        if(checkip("111.111.111.111", myip))
            throw "`ip` constructor does not intialize an IPv4 to the specifed value.";

        auto myotherip = (class :: address :: ip){"aaaa:2379:fcde::1"};
        if(checkip("aaaa:2379:fcde::1", myotherip))
            throw "`ip` constructor does not intialize an IPv6 to the specifed value.";

        auto myaddress = address("127.0.0.0", 4321);
        myip = (class address :: ip){myaddress};
        if(checkip("127.0.0.0", myip))
            throw "`ip` constructor does extract correctly the ip from the given `address`.";

        if(checkip("127.0.0.0", myip.decay()))
            throw "`decay` should not modify an IPv4.";
        if(checkip("aaaa:2379:fcde::1", myotherip.decay()))
            throw "`decay` should not modify and IPv6 which is not a representation of an IPv4 address.";
        if(checkip("192.0.2.128", address("::ffff:192.0.2.128", 1234).decay()))
            throw "`decay` should extract the corresponding IPv4 from an IPv6 representing an IPv4 address.";

        lastop = 'X';
        myip.match([&](const in_addr & ip)
        {
            lastop = '4';
        }, [&](const in6_addr & ip)
        {
            lastop = '6';
        }, [&]()
        {
            lastop = '?';
        });
        if(lastop != '4')
            throw "`match` does not execute the correct match case when called on an IPv4 `ip`.";

        lastop = 'X';
        myotherip.match([&](const in_addr & ip)
        {
            lastop = '4';
        }, [&](const in6_addr & ip)
        {
            lastop = '6';
        }, [&]()
        {
            lastop = '?';
        });
        if(lastop != '6')
            throw "`match` does not execute the correct match case when called on an IPv6 `ip`.";

        lastop = 'X';
        (class address :: ip){}.match([&](const in_addr & ip)
        {
            lastop = '4';
        }, [&](const in6_addr & ip)
        {
            lastop = '6';
        }, [&]()
        {
            lastop = '?';
        });
        if(lastop != '?')
            throw "`match` does not apply the correct match case when called on an empty `ip`.";

        // any()
        // TODO: danno 0.0.0.0 e :: ...ok? + come testo?
        // std :: cout << address :: ip :: any <IPv4> () << std :: endl;
        // std :: cout << address :: ip :: any <IPv6> () << std :: endl;

        // local()
        // TODO: come testo?
        // for(auto ip : address :: ip :: local())
        //     std :: cout << ip << std :: endl;
    });

    $test("address/port", []
    {
        auto port = (class address :: port){1234};
        if((uint16_t) port != 1234)
            throw "`port` constructor does not assign the specified value to the `port`.";

        auto addr = address("127.0.0.0", 4321);
        port = (class address :: port){addr};
        if((uint16_t) port != 4321)
            throw "`port` constructor does not extract correctly the port number from the given `address`.";
    });
};
