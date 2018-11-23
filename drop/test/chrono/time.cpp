#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "chrono/time.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("time/develop", []
    {
        for(;; sleep(1_s + 3_s))
            std :: cout << now() << std :: endl;
    });
};
