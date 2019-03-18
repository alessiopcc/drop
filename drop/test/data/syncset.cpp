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

        std :: vector <typename syncset <uint64_t> :: navigation> reference =
        {
            syncset <uint64_t> :: left, syncset <uint64_t> :: right, syncset <uint64_t> :: left, syncset <uint64_t> :: right,
            syncset <uint64_t> :: right, syncset <uint64_t> :: left, syncset <uint64_t> :: right, syncset <uint64_t> :: left,
            syncset <uint64_t> :: left, syncset <uint64_t> :: left, syncset <uint64_t> :: left, syncset <uint64_t> :: right,
            syncset <uint64_t> :: right, syncset <uint64_t> :: right, syncset <uint64_t> :: right, syncset <uint64_t> :: right
        };

        for(size_t bit = 0; bit < reference.size(); bit++)
            if(path[bit] != reference[bit])
                throw "Path bit differs from reference.";
    });

    $test("syncset/prefix", []
    {
        syncset <uint64_t> :: prefix prefix(42, 9);
        auto serialized = bytewise :: serialize(prefix);
        // auto deserialized = bytewise :: deserialize <syncset <uint64_t> :: prefix> (serialized);
        std :: cout << bytewise :: constraints :: fixed <syncset <uint64_t> :: prefix> () << std :: endl;
        std :: cout << introspection :: count <syncset <uint64_t> :: prefix, bytewise> () << std :: endl;
    });
};
