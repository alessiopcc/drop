#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/listener.hpp"
#include "network/connection.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("listener/sync", {.instances = 2},[]
    {
        if(:: test :: instance :: id() == 0)
        {
            auto one = tcp :: listen(2345);
            auto two = tcp :: listen({::test :: instance :: get <::test :: IPv4> (0), 2346});
            auto three = tcp :: listen({::test :: instance :: get <::test :: IPv6> (0), 2347});

            auto alpha = one.acceptsync();
            auto beta = two.acceptsync();
            auto gamma = three.acceptsync();
        }
        else
        {
            sleep(3);

            auto alpha = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 2345});
            auto beta = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 2346});
            auto gamma = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 2347});
        }
    });

    $test("listener/async", {.instances = 2},[]
    {
        if(:: test :: instance :: id() == 0)
        {
            auto mylistener = tcp :: listen(4323);
            auto one = mylistener.acceptsync();

            sleep(5);

            auto alpha = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 2348});
            auto beta = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 2349});
            auto gamma = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (1), 2350});

            std :: string goodbye  = "Goodbye!";
            one.sendsync(goodbye);
        }
        else
        {
            auto one = tcp :: listen(2348);
            auto two = tcp :: listen({::test :: instance :: get <::test :: IPv4> (1), 2349});
            auto three = tcp :: listen({::test :: instance :: get <::test :: IPv6> (1), 2350});

            sleep(3);

            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 4323});

            [&]() -> promise <void>
            {
                auto alpha = co_await one.acceptasync();
                auto beta = co_await two.acceptasync();
                auto gama = co_await three.acceptasync();
            }();

            goodbye.receivesync <std :: string> ();
        }
    });
};
