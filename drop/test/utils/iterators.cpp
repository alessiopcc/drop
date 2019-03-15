#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/utils/iterators.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("iterators/constraint", []
    {
        {
            auto lambda = [](auto & v)
            {
                v += 1;
            };

            if(!iterators :: constraints :: each <std :: tuple <int, char, float>, decltype(lambda)> ())
                throw "`constraints :: each` returns false for valid tuple and lambda.";
        }

        {
            auto lambda = [](int & v)
            {
                v += 1;
            };

            if(iterators :: constraints :: each <std :: tuple <int, char, float>, decltype(lambda)> ())
                throw "`constraints :: each` returns true for invalid tuple and lambda.";
        }
    });

    $test("iterators/each", []
    {
        std :: tuple <int, char, float> tuple = {1, 2, 3};

        iterators :: each(tuple, [](auto & v)
        {
            v += 1;
        });

        if(std :: get <0> (tuple) != 2 || std :: get <1> (tuple) != 3 || std :: get <2> (tuple) != 4.)
            throw "`std :: tuple <int, char, float>` is not correctly visited by `each`.";
    });
};
