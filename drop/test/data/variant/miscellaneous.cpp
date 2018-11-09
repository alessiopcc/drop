#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <type_traits>

// Includes

#include "data/variant.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;

    // Classes

    class destructible
    {
    public:

        ~destructible()
        {
            lastop = 'D';
        }
    };

    class argful
    {
    public:

        double i;

        argful(const char *, double i, const int &) : i(i)
        {
            lastop = 'P';
        }

        ~argful()
        {
            lastop = 'D';
        }
    };

    // Tests

    $test("variant/destructor", []
    {
        lastop = 'X';
        {
            variant <destructible> myvariant = destructible();
        }
        if(lastop != 'D')
            throw "When deleted, `variant <destructible>` does not call `destructible`'s destructor.'";
    });

    $test("variant/emplace", []
    {
        variant <argful, int> myvariant;

        {
            lastop = 'X';
            myvariant.emplace <argful> ("rain", 4, 99);

            if(!myvariant.is <argful> ())
                throw "`emplace` method does not initialize variant to `argful`.";
            if(myvariant.reinterpret <argful> ().i != 4.)
                throw "`emplace` method does not correctly set the value.";
            if(lastop != 'P')
                throw "`emplace` method deos not call constructor.";
        }

        {
            lastop = 'X';
            myvariant.emplace <int> (33);

            if(lastop != 'D')
                throw "`emplace` method does not call the destructor on the old value.";
            if(!myvariant.is <int> ())
                throw "`emplace` method does not update variant to `int`.";
            if(myvariant.reinterpret <int> () != 33)
                throw "`emplace` method does not correctly update the value.";
        }
    });

    $test("variant/construct", []
    {
        lastop = 'X';
        auto myvariant = variant <argful, int> :: construct <argful> ("ciao", 4, 99);

        if(!myvariant.is <argful> ())
            throw "`construct` static method does not initialize variant to `argful`.";
        if(myvariant.reinterpret <argful> ().i != 4.)
            throw "`construct` static method does not correctly set the value.";
        if(lastop != 'P')
            throw "`construct` static method deos not call constructor.";
    });
};
