#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/connection.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("connection/develop", []
    {
        /*auto connection = tcp :: connectsync({"127.0.0.1", 1234});

        connection.sendsync(std :: array <uint32_t, 10> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        auto reply = connection.receivesync <std :: array <uint32_t, 10>> ();

        for(auto value : reply)
            std :: cout << value << std :: endl;*/
    });
};
