#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/pool.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("pool/develop", []
    {
        pool mypool;

        sleep(1_s);

        tcp :: socket mysocket;
        mysocket.connect({"127.0.0.1", 1234});

        std :: cout << "Adding socket." << std :: endl;
        mypool.read(mysocket, 3_s);

        sleep(10_s);
    });
};
