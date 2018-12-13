#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/tcp.hpp"
#include "chrono/time.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("tcp/develop", []
    {
        // tcp :: socket mysocket;
        //
        // mysocket.bind(1234);
        // mysocket.listen();
        //
        // auto myothersocket = mysocket.accept();
        //
        // while(true)
        // {
        //     std :: cout << myothersocket.get <buffers :: receive :: size> () << std :: endl;
        //     std :: cout << myothersocket.get <buffers :: receive :: available> () << std :: endl;
        //     sleep(1_s);
        // }
    });
};
