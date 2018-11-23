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
        address my_address("127.0.0.1", 1234);
        std :: cout << my_address.ip() << std :: endl;
    });
};
