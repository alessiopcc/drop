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
        tcp :: socket my_socket;
        my_socket.set <timeouts :: receive> (10_s);
        std :: cout << my_socket.get <timeouts :: receive> () << std :: endl;

        my_socket.set <blocking> (false);
        std :: cout << my_socket.get <timeouts :: receive> () << std :: endl;
    });
};
