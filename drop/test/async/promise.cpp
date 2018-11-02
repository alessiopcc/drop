#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "async/promise.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    int lastval;
    int resumes;
    bool thrown;

    promise <int> x;

    // Coroutines

    promise <void> f()
    {
        lastval = co_await x;
        resumes++;
    }

    promise <void> g()
    {
        for(int i = 0; i < 10; i++)
        {
            co_await f();
            x = promise <int> ();
        }
    }

    promise <void> h()
    {
        try
        {
            co_await f();
        }
        catch(const char * exception)
        {
            if(!(strcmp(exception, "Ouch!")))
                thrown = true;
        }
    }

    // Tests

    $test("promise/resolve", []
    {
        g();

        for(int i = 33; i < 43; i++)
        {
            x.resolve(i);
            if(lastval != i)
                throw "`promise <int>` did not correctly resume coroutine `f`.";
        }

        thrown = false;

        try
        {
            x.resolve(9);
            x.resolve(10);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Calling `resolve` twice on a `promise` does not raise an exception.";

        f();
        if(lastval != 9)
            throw "Making a coroutine await on a `promise` that was already resolved does not cause it to pass through.";

        lastval = 0;

        f();
        if(lastval != 9)
            throw "Making a coroutine await twice on a `promise` that was already resolved does not cause it to pass through.";

        resumes = 0;
        x = promise <int> ();

        f();
        f();
        f();
        f();

        x.resolve(99);
        if(lastval != 99)
            throw "Resolving a `promise` on which multiple coroutines are awaiting fails.";

        if(resumes != 4)
            throw "Resolving a `promise` on which multiple coroutines are awaiting does not resume all of them.";
    });

    $test("promise/reject", []
    {
        h();

        thrown = false;
        x.reject("Ouch!");

        if(!thrown)
            throw "Rejecting a `promise` does not cause the exception to appropriately propagate.";
    });
};
