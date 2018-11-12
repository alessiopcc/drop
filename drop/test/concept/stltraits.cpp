#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "concept/stltraits.h"

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
};
