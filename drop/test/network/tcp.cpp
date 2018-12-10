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
        tcp :: socket mysocket;
        mysocket.connect({"127.0.0.1", 1235});
        std :: cout << "Successfully connected" << std :: endl;
    });
};
