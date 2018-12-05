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

        std :: cout << "Binding.." << std :: endl;
        mysocket.bind(1239);

        std :: cout << "Listening.." << std :: endl;
        mysocket.listen();

        std :: cout << "Accepting.." << std :: endl;
        auto connection = mysocket.accept();

        std :: cout << connection.remote() << std :: endl;
    });
};
