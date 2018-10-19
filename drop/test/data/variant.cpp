#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "data/variant.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;

    // Classes

    class movable
    {
    public:

        movable()
        {
        }

        movable(const movable &)
        {
            lastop = 'C';
        }

        movable(movable &&)
        {
            lastop = 'M';
        }
    };

    class copyable
    {
    public:

        copyable()
        {
        }

        copyable(const copyable &)
        {
            lastop = 'C';
        }
    };

    // Tests

    $test("variant/constructors", []
    {
        if(!(variant <int, double, char> ().is <undefined> ()))
            throw "Uninitialized `variant` is not `undefined`.";

        if(!(variant <int, double, char> (44).is <int> ()))
            throw "`variant` that was copy-initialized with `int` is not an `int`.";

        if(!(variant <int, double, float, char> (4.44).is <double> ()))
            throw "`variant` that was copy-initialized with `double` is not a `double`.";

        if(!(variant <int, double, float, char> (4.44f).is <float> ()))
            throw "`variant` that was copy-initialized with `float` is not a `float`.";

        {
            lastop = 'X';
            variant <movable> v(movable{});
            if(lastop != 'M')
                throw "In-place initialization of `variant` with `movable` does not call move constructor.";
        }

        {
            lastop = 'X';
            movable m;
            variant <movable> v(m);
            if(lastop != 'C')
                throw "Copy construction of `variant` with `movable` does not call copy constructor.";
        }

        {
            lastop = 'X';
            variant <copyable> v(copyable{});
            if(lastop != 'C')
                throw "In-place initialization of `variant` with `copyable` does not call copy constructor.";
        }

        {
            lastop = 'X';
            copyable c;
            variant <copyable> v(c);
            if(lastop != 'C')
                throw "Copy construction of `variant` with `copyable` does not call copy constructor.";
        }
    });

    $test("variant/visit", []
    {
        auto empty = []()
        {
        };

        auto acceptdouble = [](double)
        {
        };

        auto acceptanything = [](auto &&)
        {
        };

        if(variant <int, double, char> :: constraints :: visitor <decltype(empty)> ())
            throw "`variant <int, double, char>` can be visited by lambda with no arguments.";

        if(!(variant <int, double, char> :: constraints :: visitor <decltype(acceptdouble)> ()))
            throw "`variant <int, double, char>` cannot be visited by lambda with `double` argument.";

        if(variant <int, double, char, movable> :: constraints :: visitor <decltype(acceptdouble)> ())
            throw "`variant <int, double, char, movable>` can be visited by lambda with `double` argument.";

        if(!(variant <int, double, char, movable> :: constraints :: visitor <decltype(acceptanything)> ()))
            throw "`variant <int, double, char, movable>` cannot be visited by lambda with `auto` argument.";

        {
            variant <int, double, char> v = 'q';
            v.visit([](auto && value)
            {
                if(!(std :: is_same <decltype(value), char &> :: value))
                    throw "`variant` does not apply `char &` to visitor correctly.";
            });
        }

        {
            const variant <int, double, char> v = 'q';
            v.visit([](auto && value)
            {
                if(!(std :: is_same <decltype(value), const char &> :: value))
                    throw "`const variant` does not apply `const char &` to visitor correctly.";
            });
        }
    });
};
