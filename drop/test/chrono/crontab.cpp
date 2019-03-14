#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "chrono/crontab.h"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("crontab/develop", []
    {
        for(int i = 0; i < 10; i++)
            [=]() -> promise <void>
            {
                while(true)
                {
                    co_await wait(1_s);
                    std :: cout << "Hello from " << i << "!" << std :: endl;
                }
            }();

        sleep(10_s);
    });
};
