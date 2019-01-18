#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/tcp.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("tcp/develop", []
    {
        /*tcp :: listener listener(1235);

        [&]() -> promise <void>
        {
            connection connection = co_await listener.accept();
            std :: cout << "Connected!" << std :: endl;
        }();

        sleep(10_h);*/
    });
};
