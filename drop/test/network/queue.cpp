#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <thread>

// Includes

#include "network/queue.hpp"
#include "network/tcp.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("queue/develop", []
    {
        /*queue myqueue;

        tcp :: socket mysocket;
        mysocket.connect({"127.0.0.1", 1234});

        tcp :: socket myothersocket;
        myothersocket.connect({"127.0.0.1", 1234});

        myqueue.add(queue :: read, mysocket.get <descriptor> ());
        myqueue.add(queue :: read, myothersocket.get <descriptor> ());

        std :: thread mythread([&]()
        {
            sleep(1_s);
            myqueue.wake();
        });

        for(size_t i = 0; i < 100; i++)
        {
            myqueue.select([](const queue :: event & myevent)
            {
                std :: cout << "There has been an event!" << std :: endl;

                char buffer[1024];
                memset(buffer, '\0', 1024);

                read(myevent.descriptor(), buffer, 1024);

                std :: cout << "From " << myevent.descriptor() << ": " << buffer << std :: endl;
            });

            std :: cout << "Out of the select" << std :: endl;
        }

        mythread.join();*/
    });
};
