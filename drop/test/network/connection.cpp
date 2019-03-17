#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/network/connection.hpp"

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

            if(connection.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv6> (1)))
                throw "`remote` does not return the remote peer IP address.";

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

            sleep(1_s);
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 1234});

            if(connection.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv6> (0)))
                throw "`remote` does not return the remote peer IP address.";

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

            if(one.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv6> (1)))
                throw "`remote` does not return the remote peer IP address.";

            if(two.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv6> (2)))
                throw "`remote` does not return the remote peer IP address.";

            one.receivesync <std :: string> ();
            one.send(goodbye);
            two.send(goodbye);
        }
        else if(:: test :: instance :: id() == 1)
        {
            // Receiver

            auto listener = tcp :: listen({:: test :: instance :: get <:: test :: IPv4> (1), 1234});
            auto connection = listener.acceptsync();

            if(connection.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv4> (2)))
                throw "`remote` does not return the remote peer IP address.";

            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 4321});

            if(goodbye.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv6> (0)))
                throw "`remote` does not return the remote peer IP address.";

            [&]() -> promise <void>
            {
                auto mypool = pool();
                connection.bind(mypool);

                std :: string firstmessage = co_await connection.receiveasync <std :: string> ();
                std :: string secondmessage = co_await connection.receive <std :: string> ();

                connection.unbind();

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

            sleep(1_s);
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv4> (1), 1234});
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 4322});

            if(connection.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv4> (1)))
                throw "`remote` does not return the remote peer IP address.";

            if(goodbye.remote().ip() != (class address :: ip)(:: test :: instance :: get <:: test :: IPv6> (0)))
                throw "`remote` does not return the remote peer IP address.";

            auto mypool = pool();
            connection.bind(mypool);

            connection.sendasync(message);
            connection.send(message);

            connection.unbind();

            connection.sendasync(vector);
            connection.send(vector);

            goodbye.receivesync <std :: string> ();
        }
    });

    $test("connection/securesync", {.instances = 6},[]
    {
        std :: string message = "Hello world!";
        std :: vector <uint8_t> vector = {3, 2, 1, 0};

        keyexchanger local;

        if(:: test :: instance :: id() == 0)
        {
            auto listener = tcp :: listen(2234);
            auto connection = listener.acceptsync();

            connection.securesync <peer> (local);

            auto receivedmessage = connection.receivesync <std :: string> ();
            if(receivedmessage != message)
                throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

            auto receivedvector = connection.receivesync <std :: vector <uint8_t>>();
            if(receivedvector != vector)
                throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";
        }
        else if(:: test :: instance :: id() == 1)
        {
            sleep(1_s);
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 2234});

            connection.securesync <peer> (local);

            connection.sendasync(message);
            connection.sendasync(vector);
        }
        else if(:: test :: instance :: id() == 2)
        {
            auto listener = tcp :: listen(2234);
            auto connection = listener.acceptsync();

            connection.sendsync(bytewise :: serialize(local.publickey()));

            connection.securesync <server> (local);

            connection.sendasync(message);

            auto receivedvector = connection.receivesync <std :: vector <uint8_t>>();
            if(receivedvector != vector)
                throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";
        }
        else if(:: test :: instance :: id() == 3)
        {
            sleep(1_s);
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (2), 2234});

            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            connection.securesync <client> (local, remote);

            auto receivedmessage = connection.receivesync <std :: string> ();
            if(receivedmessage != message)
                throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

            connection.sendasync(vector);
        }
        else if(:: test :: instance :: id() == 4)
        {
            auto listener = tcp :: listen(2234);
            auto connection = listener.acceptsync();

            connection.sendsync(bytewise :: serialize(local.publickey()));
            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            connection.securesync <peer> (local, remote);

            connection.sendasync(message);

            auto receivedvector = connection.receivesync <std :: vector <uint8_t>>();
            if(receivedvector != vector)
                throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";
        }
        else
        {
            sleep(1_s);
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (4), 2234});

            connection.sendsync(bytewise :: serialize(local.publickey()));
            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            connection.securesync <peer> (local, remote);

            auto receivedmessage = connection.receivesync <std :: string> ();
            if(receivedmessage != message)
                throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

            connection.sendasync(vector);
        }
    });

    $test("connection/secureasync", {.instances = 7},[]
    {
        std :: string message = "Hello world!";
        std :: vector <uint8_t> vector = {3, 2, 1, 0};

        std :: string bye = "Bye.";

        keyexchanger local;

        if(:: test :: instance :: id() == 0)
        {
            auto listener = tcp :: listen(2235);
            auto connection = listener.acceptsync();

            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2235});

            [&]() -> promise <void>
            {
                co_await connection.secureasync <peer> (local);

                auto receivedmessage = co_await connection.receiveasync <std :: string> ();
                if(receivedmessage != message)
                    throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

                auto receivedvector = co_await connection.receiveasync <std :: vector <uint8_t>>();
                if(receivedvector != vector)
                    throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 1)
        {
            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2235});
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 2235});

            [&]() -> promise <void>
            {
                co_await connection.secureasync <peer> (local);

                co_await connection.sendasync(message);
                co_await connection.sendasync(vector);

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 2)
        {
            auto listener = tcp :: listen(2235);
            auto connection = listener.acceptsync();

            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2235});

            connection.sendsync(bytewise :: serialize(local.publickey()));

            [&]() -> promise <void>
            {
                co_await connection.secureasync <server> (local);

                co_await connection.sendasync(message);

                auto receivedvector = co_await connection.receiveasync <std :: vector <uint8_t>>();
                if(receivedvector != vector)
                    throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 3)
        {
            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2235});
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (2), 2235});

            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            [&]() -> promise <void>
            {
                co_await connection.secureasync <client> (local, remote);

                auto receivedmessage = co_await connection.receiveasync <std :: string> ();
                if(receivedmessage != message)
                    throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

                co_await connection.sendasync(vector);

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 4)
        {
            auto listener = tcp :: listen(2235);
            auto connection = listener.acceptsync();

            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2235});

            connection.sendsync(bytewise :: serialize(local.publickey()));
            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            [&]() -> promise <void>
            {
                co_await connection.secureasync <peer> (local, remote);

                co_await connection.sendasync(message);

                auto receivedvector = co_await connection.receiveasync <std :: vector <uint8_t>>();
                if(receivedvector != vector)
                    throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 5)
        {
            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2235});
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (4), 2235});

            connection.sendsync(bytewise :: serialize(local.publickey()));
            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            [&]() -> promise <void>
            {
                co_await connection.secureasync <peer> (local, remote);

                auto receivedmessage = co_await connection.receiveasync <std :: string> ();
                if(receivedmessage != message)
                    throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

                co_await connection.sendasync(vector);

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else
        {
            // Manager

            std :: string goodbye  = "Goodbye!";

            auto listener = tcp :: listen(2235);

            auto one = listener.acceptsync();
            auto two = listener.acceptsync();
            auto three = listener.acceptsync();
            auto four = listener.acceptsync();
            auto five = listener.acceptsync();
            auto six = listener.acceptsync();

            one.receivesync <std :: string> ();
            two.receivesync <std :: string> ();
            three.receivesync <std :: string> ();
            four.receivesync <std :: string> ();
            five.receivesync <std :: string> ();
            six.receivesync <std :: string> ();

            one.send(goodbye);
            two.send(goodbye);
            three.send(goodbye);
            four.send(goodbye);
            five.send(goodbye);
            six.send(goodbye);

            sleep(1_s);
        }
    });

    $test("connection/secure", {.instances = 7},[]
    {
        std :: string message = "Hello world!";
        std :: vector <uint8_t> vector = {3, 2, 1, 0};

        std :: string bye = "Bye.";

        keyexchanger local;

        if(:: test :: instance :: id() == 0)
        {
            auto listener = tcp :: listen(2236);
            auto connection = listener.acceptsync();

            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2236});

            [&]() -> promise <void>
            {
                co_await connection.secure <peer> (local);

                auto receivedmessage = co_await connection.receiveasync <std :: string> ();
                if(receivedmessage != message)
                    throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

                auto receivedvector = co_await connection.receiveasync <std :: vector <uint8_t>>();
                if(receivedvector != vector)
                    throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 1)
        {
            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2236});
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (0), 2236});

            [&]() -> promise <void>
            {
                co_await connection.secure <peer> (local);

                co_await connection.sendasync(message);
                co_await connection.sendasync(vector);

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 2)
        {
            auto listener = tcp :: listen(2236);
            auto connection = listener.acceptsync();

            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2236});

            connection.sendsync(bytewise :: serialize(local.publickey()));

            [&]() -> promise <void>
            {
                co_await connection.secure <server> (local);

                co_await connection.sendasync(message);

                auto receivedvector = co_await connection.receiveasync <std :: vector <uint8_t>>();
                if(receivedvector != vector)
                    throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 3)
        {
            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2236});
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (2), 2236});

            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            [&]() -> promise <void>
            {
                co_await connection.secure <client> (local, remote);

                auto receivedmessage = co_await connection.receiveasync <std :: string> ();
                if(receivedmessage != message)
                    throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

                co_await connection.sendasync(vector);

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 4)
        {
            auto listener = tcp :: listen(2236);
            auto connection = listener.acceptsync();

            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2236});

            connection.sendsync(bytewise :: serialize(local.publickey()));
            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            [&]() -> promise <void>
            {
                co_await connection.secure <peer> (local, remote);

                co_await connection.sendasync(message);

                auto receivedvector = co_await connection.receiveasync <std :: vector <uint8_t>>();
                if(receivedvector != vector)
                    throw "`receivesync <std :: vector <uint8_t>>` does not return the vector given to `sendsync`.";

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else if(:: test :: instance :: id() == 5)
        {
            sleep(1_s);
            auto goodbye = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (6), 2236});
            auto connection = tcp :: connectsync({:: test :: instance :: get <:: test :: IPv6> (4), 2236});

            connection.sendsync(bytewise :: serialize(local.publickey()));
            auto remote = bytewise :: deserialize <class keyexchanger :: publickey> (connection.receivesync <std :: array <uint8_t, bytewise :: traits :: size <class keyexchanger :: publickey> ()>> ());

            [&]() -> promise <void>
            {
                co_await connection.secure <peer> (local, remote);

                auto receivedmessage = co_await connection.receiveasync <std :: string> ();
                if(receivedmessage != message)
                    throw "`receivesync <std :: string>` does not return the string given to `sendsync`.";

                co_await connection.sendasync(vector);

                goodbye.sendsync(bye);
            }();

            goodbye.receivesync <std :: string> ();
            sleep(1_s);
        }
        else
        {
            // Manager

            std :: string goodbye  = "Goodbye!";

            auto listener = tcp :: listen(2236);

            auto one = listener.acceptsync();
            auto two = listener.acceptsync();
            auto three = listener.acceptsync();
            auto four = listener.acceptsync();
            auto five = listener.acceptsync();
            auto six = listener.acceptsync();

            one.receivesync <std :: string> ();
            two.receivesync <std :: string> ();
            three.receivesync <std :: string> ();
            four.receivesync <std :: string> ();
            five.receivesync <std :: string> ();
            six.receivesync <std :: string> ();

            one.send(goodbye);
            two.send(goodbye);
            three.send(goodbye);
            four.send(goodbye);
            five.send(goodbye);
            six.send(goodbye);

            sleep(1_s);
        }
    });

    /*$test("connection/timeout", []()
    {
        auto listener = tcp :: listen(1234);

        [&]() -> promise <void>
        {
            try
            {
                auto connection = co_await listener.accept();

                connection.set <timeouts :: receive> (10_s);
                std :: cout << co_await connection.receive <int> () << std :: endl;
            }
            catch(...)
            {
                std :: cout << "Exception" << std :: endl;
            }
        }();

        sleep(1_h);
    });*/
};
