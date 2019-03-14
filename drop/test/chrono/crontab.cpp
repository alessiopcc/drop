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
        crontab mycrontab;

        sleep(1_s);

        for(int i = 0; i < 10; i++)
            [&]() -> promise <void>
            {
                while(true)
                {
                    std :: cout << "Adding..." << std :: endl;
                    co_await mycrontab.wait(now() + 1_s);
                    std :: cout << "YAY!" << std :: endl;
                }
            }();

        sleep(10_s);
    });
};
