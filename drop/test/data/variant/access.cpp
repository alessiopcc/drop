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

    // Tests

    $test("variant/is", []
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

    $test("variant/erase", []
    {
    });

    $test("variant/assignment", []
    {
        variant <char, double, int> myvariant;
        myvariant = 4;

        if(!(myvariant.is <int> ()))
            throw "Instance assignment operator fails to set the correct type.";

        if(myvariant.reinterpret <int> () != 4)
            throw "Instance assignment operator fails to set the correct value.";

        myvariant = 5;

        if(!(myvariant.is <int> ()))
            throw "Instance assignment operator fails to keep the correct type.";

        if(myvariant.reinterpret <int> () != 5)
            throw "Instance assignment operator fails to update the correct value.";

        myvariant = 4.44;

        if(!(myvariant.is <double> ()))
            throw "Instance assignment operator fails to change to the correct type.";

        if(myvariant.reinterpret <double> () != 4.44)
            throw "Instance assignment operator fails to update the correct value when changing type.";
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
