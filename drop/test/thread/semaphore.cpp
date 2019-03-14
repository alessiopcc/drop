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

        timestamp firstwait;
        timestamp secondwait;
        timestamp thirdwait;

        std :: thread mythread([&]()
        {
            if(semaphore.wait())
                throw "`wait` without arguments returns because of a timeout.";
            if((now() - firstwait) < interval(5_s))
                throw "`wait` returns before that `post` is called on the semaphore.";

            if(semaphore.wait(3_s))
                throw "`wait` returns because of a timeout when it shouldn't.";
            if((now() - secondwait) < interval(1_s))
                throw "`wait` returns before that `post` is called on the semaphore.";

            timestamp before = now();
            if(!semaphore.wait(3_s))
                throw "`wait` does return because of a timeout whent it should.";
            timestamp after = now();
            if((after - before) < interval(3_s))
                throw "`wait` returns because of timeout before that the the specified time has elpased.";
        });

        firstwait = now();
        sleep(5_s);
        semaphore.post();

        secondwait = now();
        sleep(1_s);
        semaphore.post();

        thirdwait = now();
        sleep(5_s);
        semaphore.post();

        mythread.join();
    });
};
