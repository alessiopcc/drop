#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/pool.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("pool/develop", []
    {
        /* pool mypool;

        sleep(1_s);

        tcp :: socket mysocket = tcp :: socket :: IPv4();
        mysocket.set <blocking> (false);

        [&]() -> promise <void>
        {
            try
            {
                mysocket.connect({"128.179.183.119", 1234});

                std :: cout << "Connecting.." << std :: endl;
                co_await mypool.write(mysocket, 5_s);
                std :: cout << "Connected!" << std :: endl;

                std :: array <uint8_t, 32> data;
                streamer <receive> mystreamer(data);

                std :: cout << "Adding socket." << std :: endl;
                co_await mypool.read(mysocket, mystreamer, 10_s);

                std :: cout << "Promise resolved!" << std :: endl;
                std :: cout << data << std :: endl;
            }
            catch(const std :: exception & exception)
            {
                std :: cout << exception.what() << std :: endl;
            }
        }();

        sleep(10_m); */

        for(size_t i = 0; i < 64; i++)
            std :: cout << i << ": " << &(pool :: system.get()) << std :: endl;
    });
};
