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

        connection.sendsync(std :: array <uint32_t, 4> {1, 2, 3, 4}, std :: string("Hello World!"));
        auto [firstreply, secondreply] = connection.receivesync <std :: array <uint32_t, 4>, std :: string> ();

        for(auto value : firstreply)
            std :: cout << value << std :: endl;

        std :: cout << secondreply << std :: endl;*/
    });
};
