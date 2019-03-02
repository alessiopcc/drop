#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/connection.hpp"
#include "async/promise.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("connection/sync", {.instances = 2},[]
    {
        std :: string message = "Hello world!";
        std :: vector <uint8_t> vector = {3, 2, 1, 0};

        if(:: test :: instance :: id() == 0)
        {
            // Server

            auto listener = tcp :: listen(1234);
            auto connection = listener.acceptsync();

            auto receivedmessage = connection.receivesync <std :: string> ();
            if(receivedmessage != message)
                throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

            auto receivedvector = connection.receivesync <std :: vector <uint8_t>>();
            if(receivedvector != vector)
                throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";
        }
        else
        {
            // Client

            sleep(3);
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 1234});

            connection.sendasync(message);
            connection.sendasync(vector);
        }
    });

    $test("connection/async", {.instances = 3},[]
    {
        std :: string message = "Hello world!";
        std :: vector <uint8_t> vector = {3, 2, 1, 0};

        if(:: test :: instance :: id() == 0)
        {
            // Manager

            std :: string goodbye  = "Goodbye!";

            auto mylistener = tcp :: listen(4321);
            auto myotherlistener = tcp :: listen(4322);

            auto one = mylistener.acceptsync();
            auto two = myotherlistener.acceptsync();

            one.receivesync <std :: string> ();
            one.send(goodbye);
            two.send(goodbye);
        }
        else if(:: test :: instance :: id() == 1)
        {
            // Receiver

            auto listener = tcp :: listen(1234);
            auto connection = listener.acceptsync();

            sleep(3);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 4321});

            [&]() -> promise <void>
            {
                std :: string firstmessage = co_await connection.receiveasync <std :: string> ();
                std :: string secondmessage = co_await connection.receive <std :: string> ();

                std :: vector firstvector = co_await connection.receiveasync <std :: vector <uint8_t>>();
                std :: vector secondvector = co_await connection.receive <std :: vector <uint8_t>>();

                if(firstmessage != message)
                    throw "`receiveasync <std :: string>` does not return the string given to `sendasync`.";
                if(secondmessage != message)
                    throw "`receive <std :: string>` does not return the string given to `send`.";

                if(firstvector != vector)
                    throw "`receiveasync <std :: vector <uint8_t>>` does not return the vector given to `sendasync`.";
                if(secondvector != vector)
                    throw "`receive <std :: vector <uint8_t>>` does not return the vector given to `send`.";

                std :: string done = "Done!";
                goodbye.send(done);
            }();

            goodbye.receivesync <std :: string> ();
        }
        else
        {
            // Sender

            sleep(3);
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 1234});
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (0), 4322});

            connection.sendasync(message);
            connection.send(message);

            connection.sendasync(vector);
            connection.send(vector);

            goodbye.receivesync <std :: string> ();
        }
    });
};
