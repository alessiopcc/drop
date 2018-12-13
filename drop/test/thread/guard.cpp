#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "thread/guard.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("guard/develop", []
    {
        guard <hard> guard;

        try
        {
            std :: cout << guard([&]()
            {
                throw 44; // Works with both the throw...
                return 33; // ... and the return! (Try commenting the throw.)
            }) << std :: endl;
        }
        catch(...)
        {
            std :: cout << "Something went wrong!" << std :: endl;
        }
    });
};
