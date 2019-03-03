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
            sleep(1_s);

            auto alpha = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 2345});
            auto beta = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 2346});
            auto gamma = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 2347});
        }
    });

    $test("aalistener/async", {.instances = 2},[]
    {
        if(:: test :: instance :: id() == 0)
        {
            auto mylistener = tcp :: listen(4323);
            auto one = mylistener.acceptsync();

            sleep(1_s);

            auto alpha = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 2348});
            auto beta = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 2349});
            auto gamma = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (1), 2350});
            auto delta = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 2351});
            auto sigma = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 2352});
            auto omega = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (1), 2353});

            sleep(1_s);
            
            std :: string goodbye  = "Goodbye!";
            one.sendsync(goodbye);
        }
        else
        {
            auto one = tcp :: listen(2348);
            auto two = tcp :: listen({::test :: instance :: get <::test :: IPv4> (1), 2349});
            auto three = tcp :: listen({::test :: instance :: get <::test :: IPv6> (1), 2350});
            auto four = tcp :: listen(2351);
            auto five = tcp :: listen({::test :: instance :: get <::test :: IPv4> (1), 2352});
            auto six = tcp :: listen({::test :: instance :: get <::test :: IPv6> (1), 2353});

            sleep(1_s);

            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 4323});

            [&]() -> promise <void>
            {
                    auto alpha = co_await one.acceptasync();
                    auto beta = co_await two.acceptasync();
                    auto gamma = co_await three.acceptasync();
                    auto delta = co_await four.accept();
                    auto sigma = co_await five.accept();
                    auto omega = co_await six.accept();
            }();

            goodbye.receivesync <std :: string> ();
        }
    });
};
