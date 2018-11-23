#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/address.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("address/develop", []
    {
        for(auto ip : address :: ip :: local())
            std :: cout << ip << std :: endl;
    });
};
