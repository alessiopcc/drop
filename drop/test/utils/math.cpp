#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/utils/math.hpp"

namespace
{
    // Using

    using namespace drop;

    // Methods

    template <int value> int identity()
    {
        return value;
    };

    // Tests

    $test("math/max", []
    {
        if(identity <max({1, 3, 2, -1})> () != 3)
            throw "`max({1, 3, 1})` does not return 3.`";

        if(identity <max({sizeof(int), sizeof(short), sizeof(char)})> () != 4)
            throw "`max(sizeof(int), sizeof(short), sizeof(char))` does not return 4.`";

        if(identity <max({-1, -3, -2, -5})> () != -1)
            throw "`max({1, 3, 1})` does not return -1.`";

        if(identity <max({'r', 'a', 'i', 'n'})> () != (int)'r')
            throw "`max({'r', 'a', 'i', 'n'})` does not return `(int)'r'`";
    });
};
