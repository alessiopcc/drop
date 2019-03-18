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
        for(size_t depth = 0; depth < hash :: size * 8; depth++)
        {
            syncset <uint64_t> :: prefix prefix(42, depth);

            auto serialized = bytewise :: serialize(prefix);

            if(serialized.size() != ((depth >= 128 ? 2 : 1) + (depth + 7) / 8))
                throw "Serialized prefix too long.";

            auto deserialized = bytewise :: deserialize <syncset <uint64_t> :: prefix> (serialized);

            for(size_t index = 0; index < depth; index++)
                if(deserialized[index] != prefix[index])
                    throw "Deserialized prefix does not match original prefix.";
        }

        {
            std :: vector <syncset <uint64_t> :: navigation> reference =
            {
                syncset <uint64_t> :: left, syncset <uint64_t> :: right, syncset <uint64_t> :: left, syncset <uint64_t> :: right,
                syncset <uint64_t> :: right, syncset <uint64_t> :: left, syncset <uint64_t> :: right, syncset <uint64_t> :: left,
                syncset <uint64_t> :: left, syncset <uint64_t> :: left, syncset <uint64_t> :: left, syncset <uint64_t> :: right,
                syncset <uint64_t> :: right, syncset <uint64_t> :: right, syncset <uint64_t> :: right, syncset <uint64_t> :: right
            };

            syncset <uint64_t> :: prefix prefix(hash(), 0);

            for(size_t depth = 0; depth < reference.size(); depth++)
            {
                if(reference[depth] == syncset <uint64_t> :: left)
                    prefix = prefix.left();
                else
                    prefix = prefix.right();

                if(prefix.depth() != depth + 1)
                    throw "Prefix has wrong depth.";

                for(size_t index = 0; index <= depth; index++)
                    if(prefix[index] != reference[index])
                        throw "Prefix has wrong bits.";
            }
        }
    });
};
