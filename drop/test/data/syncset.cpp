#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/data/syncset.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("syncset/path", []
    {
        // hash(42) = 5a1f.. = 0101 1010 0001 1111..

        syncset <uint64_t> :: path path(42);

        std :: vector <bool> reference = {false, true, false, true, true, false, true, false, false, false, false, true, true, true, true, true};
        for(size_t bit = 0; bit < reference.size(); bit++)
            if(path[bit] != reference[bit])
                throw "Path bit differs from reference.";
    });
};
