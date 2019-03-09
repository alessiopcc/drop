#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/tcp.hpp"
#include "network/connection.hpp"

namespace
{
    // Using

    using namespace drop;

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

    $test("tcp/sync", {.instances = 2}, []
    {
        if(:: test :: instance :: id() == 0)
        {
            auto one = tcp :: listen(5345);
            auto two = tcp :: listen({::test :: instance :: get <::test :: IPv4> (0), 5346});
            auto three = tcp :: listen({::test :: instance :: get <::test :: IPv6> (0), 5347});

            auto alpha = one.acceptsync();
            auto beta = two.acceptsync();
            auto gamma = three.acceptsync();
        }
        else
        {
            sleep(1_s);

            auto alpha = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 5345});
            auto beta = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 5346});
            auto gamma = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 5347});
        }
    });

    $test("tcp/async", {.instances = 2}, []
    {
        if(:: test :: instance :: id() == 0)
        {
            pool mypool;

            sleep(1_s);

            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (1), 5323});

            sleep(1_s);

            [&]() -> promise <void>
            {
                auto alpha = co_await tcp :: connectasync({:: test :: instance :: get <:: test :: IPv6> (1), 5348});
                auto beta = co_await tcp :: connectasync({:: test :: instance :: get <:: test :: IPv4> (1), 5349});
                auto gamma = co_await tcp :: connectasync({:: test :: instance :: get <:: test :: IPv6> (1), 5350});
                auto delta = co_await tcp :: connectasync({:: test :: instance :: get <:: test :: IPv6> (1), 5351}, mypool);
                auto sigma = co_await tcp :: connectasync({:: test :: instance :: get <:: test :: IPv4> (1), 5352}, mypool);
                auto omega = co_await tcp :: connectasync({:: test :: instance :: get <:: test :: IPv6> (1), 5353}, mypool);
            }();

            [&]() -> promise <void>
            {
                auto alpha = co_await tcp :: connect({:: test :: instance :: get <:: test :: IPv6> (1), 6348});
                auto beta = co_await tcp :: connect({:: test :: instance :: get <:: test :: IPv4> (1), 6349});
                auto gamma = co_await tcp :: connect({:: test :: instance :: get <:: test :: IPv6> (1), 6350});
                auto delta = co_await tcp :: connect({:: test :: instance :: get <:: test :: IPv6> (1), 6351}, mypool);
                auto sigma = co_await tcp :: connect({:: test :: instance :: get <:: test :: IPv4> (1), 6352}, mypool);
                auto omega = co_await tcp :: connect({:: test :: instance :: get <:: test :: IPv6> (1), 6353}, mypool);
            }();

            goodbye.receivesync <std :: string> ();
        }
        else
        {
            auto mylistener = tcp :: listen(5323);
            auto control = mylistener.acceptsync();

            auto one = tcp :: listen(5348);
            auto two = tcp :: listen({::test :: instance :: get <::test :: IPv4> (1), 5349});
            auto three = tcp :: listen({::test :: instance :: get <::test :: IPv6> (1), 5350});
            auto four = tcp :: listen(5351);
            auto five = tcp :: listen({::test :: instance :: get <::test :: IPv4> (1), 5352});
            auto six = tcp :: listen({::test :: instance :: get <::test :: IPv6> (1), 5353});

            auto alpha = tcp :: listen(6348);
            auto beta = tcp :: listen({::test :: instance :: get <::test :: IPv4> (1), 6349});
            auto gamma = tcp :: listen({::test :: instance :: get <::test :: IPv6> (1), 6350});
            auto delta = tcp :: listen(6351);
            auto sigma = tcp :: listen({::test :: instance :: get <::test :: IPv4> (1), 6352});
            auto omega = tcp :: listen({::test :: instance :: get <::test :: IPv6> (1), 6353});

            sleep(1_s);

            one.acceptsync();
            two.acceptsync();
            three.acceptsync();
            four.acceptsync();
            five.acceptsync();
            six.acceptsync();

            alpha.acceptsync();
            beta.acceptsync();
            gamma.acceptsync();
            delta.acceptsync();
            sigma.acceptsync();
            omega.acceptsync();

            sleep(1_s);

            std :: string goodbye  = "Goodbye!";
            control.sendsync(goodbye);
        }
    });

    $test("tcp/socket-setup", []
    {
        auto ipv4 = tcp :: socket :: IPv4();
        auto ipv6 = tcp :: socket :: IPv6();
        auto any = tcp :: socket :: any();

        ipv4.set <blocking> (false);
        ipv6.set <blocking> (false);
        any.set <blocking> (false);
        if(ipv4.get <blocking> () || ipv6.get <blocking> () || any.get <blocking> ())
            throw "`get <blocking>` does not return `false` after `set <blocking> (false)`.";

        ipv4.set <blocking> (true);
        ipv6.set <blocking> (true);
        any.set <blocking> (true);
        if(!ipv4.get <blocking> () || !ipv6.get <blocking> () || !any.get <blocking> ())
            throw "`get <blocking>` does not return `true` after `set <blocking> (true)`.";

        ipv4.set <reuse> (false);
        ipv6.set <reuse> (false);
        any.set <reuse> (false);
        if(ipv4.get <reuse> () || ipv6.get <reuse> () || any.get <reuse> ())
            throw "`get <reuse>` does not return `false` after `set <reuse> (false)`.";

        ipv4.set <reuse> (true);
        ipv6.set <reuse> (true);
        any.set <reuse> (true);
        if(!ipv4.get <reuse> () || !ipv6.get <reuse> () || !any.get <reuse> ())
            throw "`get <reuse>` does not return `true` after `set <reuse> (true)`.";

        if(ipv4.get <domain> () != PF_INET)
            throw "`get <domain>` does not return `PF_INET` when called on an IPv4 socket.";

        if(ipv6.get <domain> () != PF_INET6)
            throw "`get <domain>` does not return `PF_INET6` when called on an IPv6 socket.";


        ipv4.set <timeouts :: send> (interval(7600000));
        ipv6.set <timeouts :: send> (interval(7600000));
        any.set <timeouts :: send> (interval(7600000));

        // With some values `get` returns a value which is a few milliseconds higher than the one passe to `set` (not our fault).
        bool a = ((ipv4.get <timeouts :: send> () - interval(7600000)) > interval(10000)) || (ipv4.get <timeouts :: send> () < interval(7600000));
        bool b = ((ipv6.get <timeouts :: send> () - interval(7600000)) > interval(10000)) || (ipv6.get <timeouts :: send> () < interval(7600000));
        bool c = ((any.get <timeouts :: send> () - interval(7600000)) > interval(10000)) || (any.get <timeouts :: send> () < interval(7600000));

        if(a || b || c)
            throw "A `get <timeouts :: send>` does not return the value set with `set <timeouts :: send>`.";

        ipv4.set <timeouts :: receive> (interval(7610000));
        ipv6.set <timeouts :: receive> (interval(7610000));
        any.set <timeouts :: receive> (interval(7610000));

        a = ((ipv4.get <timeouts :: receive> () - interval(7610000)) > interval(10000)) || (ipv4.get <timeouts :: receive> () < interval(7610000));
        b = ((ipv6.get <timeouts :: receive> () - interval(7610000)) > interval(10000)) || (ipv6.get <timeouts :: receive> () < interval(7610000));
        c = ((any.get <timeouts :: receive> () - interval(7610000)) > interval(10000)) || (any.get <timeouts :: receive> () < interval(7610000));

        if(a || b || c)
            throw "B `get <timeouts :: receive>` does not return the value set with `set <timeouts :: receive>`.";

        // TODO do better tests...HOW -> send e vedi se c'è dentro qualcosa
        a = (ipv4.get <buffers :: send :: size> () - ipv4.get <buffers :: send :: pending> ()) == ipv4.get <buffers :: send :: available> ();
        b = (ipv6.get <buffers :: send :: size> () - ipv6.get <buffers :: send :: pending> ()) == ipv6.get <buffers :: send :: available> ();
        c = (any.get <buffers :: send :: size> () - any.get <buffers :: send :: pending> ()) == any.get <buffers :: send :: available> ();
        if(!(a && b && c))
            throw "`get <buffers :: send :: available>` is not equal to `get <buffers :: send :: size>` minus `get <buffers :: send :: pending>`.";

        a = ipv4.get <buffers :: receive :: size> () >= ipv4.get <buffers :: receive :: available> ();
        b = ipv6.get <buffers :: receive :: size> () >= ipv6.get <buffers :: receive :: available> ();
        c = any.get <buffers :: receive :: size> () >= any.get <buffers :: receive :: available> ();
        if(!(a && b && c))
            throw "`get <buffers :: receive :: available>` is not equal to `get <buffers :: receive :: size>` minus `get <buffers :: receive :: pending>`.";
    });

    $test("tcp/socket-ipv4", {.instances = 2}, []
    {
        if(:: test :: instance :: id() == 0)
        {
            auto socket = tcp :: socket :: IPv4();
            socket.bind(1234);
            socket.listen();
            auto connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (1), connection.remote().ip()))
            throw "`remote` does not return the correct address.";

            std :: array <uint8_t, 1> buffer = {65};
            connection.send(buffer.data(), 1);

            sleep(1_s);

            connection.receive(buffer.data(), 1);
            if(buffer[0] != 'B')
                throw "Over an IPv4 socket, `receive` does not return the sent value.";

            connection.close();
            socket.close();
            if(socket || connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket = tcp :: socket :: IPv4();
            socket.bind({:: test :: instance :: get <:: test :: IPv4> (0), 1234});
            socket.listen();
            connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (1), connection.remote().ip()))
            throw "`remote` does not return the correct address.";

            buffer[0] = 67;
            connection.send(buffer.data(), 1);

            sleep(1_s);

            connection.receive(buffer.data(), 1);
            if(buffer[0] != 'D')
                throw "Over an IPv4 socket, `receive` does not return the sent value.";

            connection.close();
            if(connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (1), connection.remote().ip()))
            throw "`remote` does not return the correct address.";

            buffer[0] = 69;
            connection.send(buffer.data(), 1);

            connection.close();
            socket.close();
            if(socket || connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";
        }
        else
        {
            auto socket = tcp :: socket :: IPv4();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv4> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            std :: array <uint8_t, 1> buffer;
            socket.receive(buffer.data(), 1);
            if(buffer[0] != 'A')
                throw "Over an IPv4 socket, `receive` does not return the sent value.";

            sleep(1_s);

            buffer[0] = 66;
            socket.send(buffer.data(), 1);

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket = tcp :: socket :: IPv4();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv4> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            socket.receive(buffer.data(), 1);
            if(buffer[0] != 'C')
                throw "Over an IPv4 socket, `receive` does not return the sent value.";

            sleep(1_s);

            buffer[0] = 68;
            socket.send(buffer.data(), 1);

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket = tcp :: socket :: IPv4();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv4> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            socket.receive(buffer.data(), 1);
            if(buffer[0] != 'E')
                throw "Over an IPv4 socket, `receive` does not return the sent value.";

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";
        }
    });

    $test("tcp/socket-ipv6", {.instances = 2}, []
    {
        if(:: test :: instance :: id() == 0)
        {
            auto socket = tcp :: socket :: IPv6();
            socket.bind(1234);
            socket.listen();
            auto connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (1), connection.remote().ip()))
                throw "`remote` does not return the correct address.";

            std :: array <uint8_t, 1> buffer = {65};
            connection.send(buffer.data(), 1);

            sleep(1_s);

            connection.receive(buffer.data(), 1);
            if(buffer[0] != 'B')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            connection.close();
            socket.close();
            if(socket || connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket = tcp :: socket :: IPv6();
            socket.bind({:: test :: instance :: get <:: test :: IPv6> (0), 1234});
            socket.listen();
            connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (1), connection.remote().ip()))
                throw "`remote` does not return the correct address.";

            buffer[0] = 67;
            connection.send(buffer.data(), 1);

            sleep(1_s);

            connection.receive(buffer.data(), 1);
            if(buffer[0] != 'D')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            connection.close();
            if(connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (1), connection.remote().ip()))
                throw "`remote` does not return the correct address.";

            buffer[0] = 69;
            connection.send(buffer.data(), 1);

            sleep(1_s);

            connection.close();
            socket.close();
            if(socket || connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";
        }
        else
        {
            auto socket = tcp :: socket :: any();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv6> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            std :: array <uint8_t, 1> buffer;
            socket.receive(buffer.data(), 1);

            if(buffer[0] != 'A')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            sleep(1_s);

            buffer[0] = 66;
            socket.send(buffer.data(), 1);

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket = tcp :: socket :: IPv6();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv6> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            socket.receive(buffer.data(), 1);

            if(buffer[0] != 'C')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            sleep(1_s);

            buffer[0] = 68;
            socket.send(buffer.data(), 1);

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket = tcp :: socket :: IPv6();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv6> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            socket.receive(buffer.data(), 1);

            if(buffer[0] != 'E')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";
        }
    });

    $test("tcp/socket-any", {.instances = 2}, []
    {
        if(:: test :: instance :: id() == 0)
        {
            auto socket = tcp :: socket :: any();
            socket.bind(1234);
            socket.listen();
            auto connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (1), connection.remote().ip()))
                throw "`remote` does not return the correct address.";

            std :: array <uint8_t, 1> buffer = {65};
            connection.send(buffer.data(), 1);

            sleep(1_s);

            connection.receive(buffer.data(), 1);
            if(buffer[0] != 'B')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            connection.close();
            if(connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            connection = socket.accept();

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (1), connection.remote().ip()))
                throw "`remote` does not return the correct address.";

            buffer[0] = 67;
            connection.send(buffer.data(), 1);

            sleep(1_s);

            connection.receive(buffer.data(), 1);
            if(buffer[0] != 'D')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            connection.close();
            if(connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket.close();
            if(socket || connection)
                throw "`socket` still has a valid `descriptor` after `close` is called.";
        }
        else
        {
            auto socket = tcp :: socket :: IPv4();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv4> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv4> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            std :: array <uint8_t, 1> buffer;
            socket.receive(buffer.data(), 1);

            if(buffer[0] != 'A')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            sleep(1_s);

            buffer[0] = 66;
            socket.send(buffer.data(), 1);

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";

            socket = tcp :: socket :: IPv6();

            sleep(1_s);

            socket.connect({:: test :: instance :: get <:: test :: IPv6> (0), 1234});

            if(checkip(:: test :: instance :: get <:: test :: IPv6> (0), socket.remote().ip()))
                throw "`remote` does not return the correct address.";

            socket.receive(buffer.data(), 1);

            if(buffer[0] != 'C')
                throw "Over an IPv6 socket, `receive` does not return the sent value.";

            sleep(1_s);

            buffer[0] = 68;
            socket.send(buffer.data(), 1);

            socket.close();
            if(socket)
                throw "`socket` still has a valid `descriptor` after `close` is called.";
        }
    });
};
