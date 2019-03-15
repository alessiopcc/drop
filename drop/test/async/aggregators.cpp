#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/async/aggregators.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("aggregators/develop-all", []
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

    $test("aggregators/develop-any", []
    {
        promise <int> a, b, c;
        promise <double> d;

        [&]() -> promise <void>
        {
            try
            {
                std :: cout << co_await any(a, b, c) << std :: endl;
            }
            catch(const std :: array <std :: exception_ptr, 3> & exceptions)
            {
                try
                {
                    std :: rethrow_exception(exceptions[2]);
                }
                catch(const char * message)
                {
                    std :: cout << message << std :: endl;
                }
            }
        }();

        std :: cout << "Rejecting c.." << std :: endl;
        c.reject("and your stupid code!");

        std :: cout << "Rejecting a.." << std :: endl;
        a.reject("screw you!");

        // std :: cout << "Rejecting b.." << std :: endl;
        // b.reject("and your promise!");

        std :: cout << "Resolving b.." << std :: endl;
        b.resolve(33);
    });
};
