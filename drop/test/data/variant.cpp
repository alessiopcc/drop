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

    $test("variant/match", []
    {
    });

    $test("variant/reinterpret", []
    {
    });

    $test("variant/get", []
    {
        variant <int, float, char> myvariant = 'c';

        try
        {
            myvariant.get <char> () = 'q';
        }
        catch(...)
        {
            throw "`myvariant.get <char> ()` raises an exception.";
        }

        if(myvariant.get <char> () != 'q')
            throw "`myvariant.get <char> ()` does not retrieve or set the value appropriately.";

        bool except = false;

        try
        {
            myvariant.get <float> () = 4.44;
        }
        catch(exception <bad_access, type_mismatch> &)
        {
            except = true;
        }

        if(!except)
            throw "`myvariant.get <float> ()` does not raise an exception.";
    });

    $test("variant/cast", []
    {
        variant <int> emptyvariant;
        variant <int> fullvariant = 33;

        if(emptyvariant)
            throw "Undefined `variant` casts to boolean `true`.";

        if(!fullvariant)
            throw "Non-undefined `variant` casts to boolean `false`.";
    });
};
