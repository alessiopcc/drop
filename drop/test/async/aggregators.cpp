#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/async/aggregators.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("aggregators/all", []
    {
        {
            promise <void> a, b, c;
            bool resolved = false;

            [&]() -> promise <void>
            {
                co_await all(a, b, c);
                resolved = true;
            }();

            a.resolve();
            if(resolved)
                throw "Aggregator `all` resolves the aggregated promise before all promises are resolved.";

            b.resolve();
            if(resolved)
                throw "Aggregator `all` resolves the aggregated promise before all promises are resolved.";

            c.resolve();
            if(!resolved)
                throw "Aggregator `all` does not resolve the aggregated promise when all promises are resolved.";
        }

        {
            promise <void> a, b, c;
            bool rejected = false;

            [&]() -> promise <void>
            {
                try
                {
                    co_await all(a, b, c);
                }
                catch(...)
                {
                    rejected = true;
                }
            }();

            a.resolve();
            if(rejected)
                throw "Aggregator `all` rejects the aggregated promise before any promise is rejected.";

            c.reject("surprise");
            if(rejected)
                throw "Aggregator `all` does not check promises in appropriate sequence.";

            b.resolve();
            if(!rejected)
                throw "Aggregator `all` does not reject the aggregated promise when a promise is rejected.";
        }
    });

    $test("aggregators/any", []
    {
        {
            promise <int> a, b, c;
            optional <int> value;

            [&]() -> promise <void>
            {
                value = co_await any(a, b, c);
            }();

            c.resolve(11);
            if(!value || *value != 11)
                throw "Aggregator `any` does not appropriately resolve the aggregated promise when the first promise is resolved.";
        }

        {
            promise <int> a, b, c;
            optional <std :: array <std :: exception_ptr, 3>> rejected;

            [&]() -> promise <void>
            {
                try
                {
                    int value = co_await any(a, b, c);
                }
                catch(const std :: array <std :: exception_ptr, 3> exceptions)
                {
                    rejected = exceptions;
                }
            }();

            c.reject(13);
            if(rejected)
                throw "Aggregator `any` rejects the aggregated promise before all promises are rejected";

            a.reject(11);
            if(rejected)
                throw "Aggregator `any` rejects the aggregated promise before all promises are rejected";

            b.reject(12);
            if(!rejected)
                throw "Aggregator `any` does not reject the aggregated promise when all promises are rejected";

            struct {int a; int b; int c;} exceptions;

            try
            {
                std :: rethrow_exception((*rejected)[0]);
            }
            catch(const int & exception)
            {
                exceptions.a = exception;
            }

            try
            {
                std :: rethrow_exception((*rejected)[1]);
            }
            catch(const int & exception)
            {
                exceptions.b = exception;
            }

            try
            {
                std :: rethrow_exception((*rejected)[2]);
            }
            catch(const int & exception)
            {
                exceptions.c = exception;
            }

            if(exceptions.a != 11 || exceptions.b != 12 || exceptions.c != 13)
                throw "Aggregator `any` does not appropriately forward the exceptions to the rejection of the aggregated promise.";
        }
    });
};
