#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/concept/stltraits.h"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class myclass
    {
    };

    // Tests

    $test("stltraits/array", []
    {
        if(!(stltraits :: array <std :: array <int, 4>> :: value))
            throw "`std :: array <int, 4>` is not recognized as a `std :: array`.";

        if(!(std :: is_same <stltraits :: array <std :: array <int, 4>> :: type, int> :: value))
            throw "`std :: array <int, 4>` is not diagnosed to be an array of `int`s.";

        if(stltraits :: array <std :: array <int, 4>> :: size != 4)
            throw "`std :: array <int, 4>` is not diagnosed to have 4 elements.";

        if(stltraits :: array <int> :: value)
            throw "`int` is recognized as a `std :: array`.";
    });

    $test("stltraits/vector", []
    {
        if(!(stltraits :: vector <std :: vector <int>> :: value))
            throw "`std :: vector <int, 4>` is not recognized as a `std :: vector`.";

        if(!(std :: is_same <stltraits :: vector <std :: vector <int>> :: type, int> :: value))
            throw "`std :: vector <int>` is not diagnosed to be a vector of `int`s.";

        if(stltraits :: vector <int> :: value)
            throw "`int` is recognized as a `std :: vector`.";
    });

    $test("stltraits/tuple", []
    {
        if(!(stltraits :: tuple <std :: tuple <int>> :: value))
            throw "`std :: tuple <int>` is not recognized as a `std :: tuple`.";

        if(stltraits :: tuple <std :: tuple <int>> :: size != 1)
            throw "`std :: tuple <int>` is not recognized to have one element.";

        if(!(stltraits :: tuple <std :: tuple <int, float>> :: value))
            throw "`std :: tuple <int, float>` is not recognized as a `std :: tuple`.";

        if(stltraits :: tuple <std :: tuple <int, float>> :: size != 2)
            throw "`std :: tuple <int, float>` is not recognized to have two elements.";

        if(!(std :: is_same <stltraits :: tuple <std :: tuple <int, char, float>> :: type <1>, char> :: value))
            throw "The second type of `std :: tuple <int, char, float>` is not recognized to be `char`.";
    });
};
