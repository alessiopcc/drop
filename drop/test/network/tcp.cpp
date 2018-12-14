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
        /*tcp :: socket mysocket;

        mysocket.bind(1235);
        mysocket.listen();

        auto myothersocket = mysocket.accept();
        myothersocket.set <timeouts :: receive> (5_s);

        const char * hello = "Hello World!";
        myothersocket.send((const uint8_t *) hello, strlen(hello));

        char * reply = new char[1024];
        myothersocket.receive((uint8_t *) reply, 1024);

        std :: cout << reply << std :: endl;*/
    });
};
