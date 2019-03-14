#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "async/aggregators.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("aggregators/develop", []
    {
        promise <void> a, b, c, d;

        [&]() -> promise <void>
        {
            co_await all(a, b, c, d);
            std :: cout << "Completed!" << std :: endl;
        }();

        std :: cout << "A" << std :: endl;
        a.resolve();

        std :: cout << "B" << std :: endl;
        b.resolve();

        std :: cout << "C" << std :: endl;
        c.resolve();

        std :: cout << "D" << std :: endl;
        d.resolve();
    });
};
