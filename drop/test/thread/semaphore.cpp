#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <thread>

// Includes

#include "thread/semaphore.h"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("semaphore/develop", []
    {
        semaphore semaphore;

        std :: thread mythread([&]()
        {
            std :: cout << "Waiting..." << std :: endl;
            if(semaphore.wait(3_s))
                std :: cout << "Out because of timeout" << std :: endl;
            else
                std :: cout << "Out because of post" << std :: endl;
        });

        sleep(4_s);
        semaphore.post();
        mythread.join();
    });
};
