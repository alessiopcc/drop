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

    $test("pool/develop", []
    {
        /*[]() -> promise <void>
        {
            std :: cout << "Connecting" << std :: endl;
            connection myconnection = co_await tcp :: connect({"127.0.0.1", 1234});
            std :: cout << "Connected" << std :: endl;

            std :: cout << "Sending" << std :: endl;
            co_await myconnection.send(std :: string("Hello World!"));
            std :: cout << "Sent" << std :: endl;

            std :: cout << "Receiving" << std :: endl;
            auto reply = co_await myconnection.receive <std :: string> ();
            std :: cout << "Received " << reply << std :: endl;
        }();

        sleep(1_h);*/
    });
};
