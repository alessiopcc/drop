#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/async/promise.hpp"

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

    $test("promise/resolvehard", []
    {
        g();

        for(int i = 33; i < 43; i++)
        {
            x.resolvehard(i);
            if(lastval != i)
                throw "Calling `resolvehard` on `promise <int>` did not correctly resume coroutine `f`.";
        }

        thrown = false;

        try
        {
            x.resolvehard(9);
            x.resolvehard(10);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Calling `resolvehard` twice on a `promise` does not raise an exception.";

        f();
        if(lastval != 9)
            throw "Making a coroutine await on a `promise` that was already hard-resolved does not cause it to pass through.";

        lastval = 0;

        f();
        if(lastval != 9)
            throw "Making a coroutine await twice on a `promise` that was already hard-resolved does not cause it to pass through.";

        resumes = 0;
        x = promise <int> ();

        f();
        f();
        f();
        f();

        x.resolvehard(99);
        if(lastval != 99)
            throw "Hard-esolving a `promise` on which multiple coroutines are awaiting fails.";

        if(resumes != 4)
            throw "Hard-resolving a `promise` on which multiple coroutines are awaiting does not resume all of them.";
    });

    $test("promise/resolvesoft", []
    {
        g();

        for(int i = 33; i < 43; i++)
        {
            x.resolvesoft(i);
            if(lastval != i)
                throw "Calling `resolvesoft` on `promise <int>` did not correctly resume coroutine `f`.";
        }

        thrown = false;
        try
        {
            x.resolve(9);
            x.resolvesoft(10);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `resolvesoft` on `promise` that was previously resolved raises an exception.";

        x = promise <int> ();
        h();
        thrown = false;
        try
        {
            x.rejectsoft("Soft");
            x.resolvesoft(10);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `resolvesoft` on a `promise` that was previously soft-rejected raises an exception.";

        x = promise <int> ();
        h();
        thrown = false;
        try
        {
            x.reject("Soft");
            x.resolvesoft(10);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `resolvesoft` on a `promise` that was previously rejected raises an exception.";

        x = promise <int> ();
        thrown = false;
        try
        {
            x.resolvesoft(9);
            x.resolvesoft(10);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `resolvesoft` twice on a `promise` raises an exception.";

        f();
        if(lastval != 9)
            throw "Making a coroutine await on a `promise` that was already soft-resolved does not cause it to pass through.";

        lastval = 0;

        f();
        if(lastval != 9)
            throw "Making a coroutine await twice on a `promise` that was already soft-resolved does not cause it to pass through.";

        resumes = 0;
        x = promise <int> ();

        f();
        f();
        f();
        f();

        x.resolvesoft(99);
        if(lastval != 99)
            throw "Soft-esolving a `promise` on which multiple coroutines are awaiting fails.";

        if(resumes != 4)
            throw "Soft-resolving a `promise` on which multiple coroutines are awaiting does not resume all of them.";
    });

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

    $test("promise/rejecthard", []
    {
        h();

        thrown = false;
        x.rejecthard("Ouch!");

        if(!thrown)
            throw "Hard-rejecting a `promise` does not cause the exception to appropriately propagate.";

        thrown = false;
        x = promise <int> ();
        x.rejecthard("Ouch!");

        h();

        if(!thrown)
            throw "Making a coroutine await on an already hard-rejected `promise` does not cause it to pass through.";

        thrown = false;

        try
        {
            x.rejecthard(33);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Rejecting a `promise` twice does not raise an exception.";

        thrown = false;

        try
        {
            x.resolve(144);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Resolving a `promise` that was previously hard-rejected does not raise an exception.";
    });

    $test("promise/rejectsoft", []
    {
        h();

        thrown = false;
        x.rejectsoft("Ouch!");

        if(!thrown)
            throw "Soft-rejecting a `promise` does not cause the exception to appropriately propagate.";

        thrown = false;
        x = promise <int> ();
        x.rejectsoft("Ouch!");

        h();

        if(!thrown)
            throw "Making a coroutine await on an already soft-rejected `promise` does not cause it to pass through.";

        thrown = false;

        try
        {
            x.rejectsoft(33);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(thrown)
            throw "Soft-rejecting a `promise` twice raises an exception.";

        thrown = false;

        try
        {
            x.resolve(144);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Resolving a `promise` that was previously soft-rejected does not raise an exception.";

        x = promise <int> ();
        thrown = false;
        try
        {
            x.resolve(9);
            x.rejectsoft("Soft");
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `rejectsoft` on a `promise` that was previously resolved raises an exception.";

        x = promise <int> ();
        h();
        thrown = false;
        try
        {
            x.rejectsoft("Soft");
            x.resolvesoft(10);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `resolvesoft` twice a `promise` raises an exception.";

        x = promise <int> ();
        h();
        thrown = false;
        try
        {
            x.reject("Soft");
            x.rejectsoft("Soft");
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `rejectsoft` on a `promise` that was previously rejected raises an exception.";

        x = promise <int> ();
        thrown = false;
        try
        {
            x.resolvesoft(9);
            x.rejectsoft("Soft");
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }
        if(thrown)
            throw "Calling `rejectsoft` twice on a `promise` that was previously soft-resolced raises an exception.";
    });

    $test("promise/reject", []
    {
        h();

        thrown = false;
        x.reject("Ouch!");

        if(!thrown)
            throw "Rejecting a `promise` does not cause the exception to appropriately propagate.";

        thrown = false;
        x = promise <int> ();
        x.reject("Ouch!");

        h();

        if(!thrown)
            throw "Making a coroutine await on an already rejected `promise` does not cause it to pass through.";

        thrown = false;

        try
        {
            x.reject(33);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Rejecting a `promise` twice does not raise an exception.";

        thrown = false;

        try
        {
            x.resolve(144);
        }
        catch(exception <bad_access, already_resolved> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Resolving a `promise` that was previously rejected does not raise an exception.";
    });

    $test("promise/unhandled-exception", []
    {
        {
            std :: set_terminate([]
            {
                std :: exit(0);
            });

            promise <void> y;
            y.reject("Ouch!");
        }
    });
};
