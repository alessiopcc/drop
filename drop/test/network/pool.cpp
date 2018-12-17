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
        // Client

        /*auto acceptor = tcp :: socket :: IPv4();
        acceptor.bind({"127.0.0.1", 1236});

        acceptor.listen();
        auto socket = acceptor.accept();

        connection connection(socket);*/

        // Server

        // auto connection = tcp :: connectsync({"127.0.0.1", 1236});

        // Chat code

        /*[&]() -> promise <void>
        {
            while(true)
            {
                std :: cout << co_await connection.receiveasync <std :: string> () << std :: endl;
            }
        }();

        while(true)
        {
            char buffer[1024];
            std :: cin.getline(buffer, 1024);

            std :: string message(buffer);
            connection.sendasync(message);
        }

        sleep(1_h);*/
    });
};
