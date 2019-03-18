#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <unordered_set>

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

    $test("syncset/node", []
    {
        syncset <uint64_t> :: node root = syncset <uint64_t> :: empty();
        root = root.reinterpret <syncset <uint64_t> :: empty> ().fill(42);

        bool success = false;
        root.match([&](syncset <uint64_t> :: single & root)
        {
            if(root.element() == 42)
                success = true;
        });

        if(!success)
            throw "`empty.fill()` on root does not work.";

        root = root.reinterpret <syncset <uint64_t> :: single> ().push(syncset <uint64_t> :: left);

        success = false;
        root.match([&](syncset <uint64_t> :: multiple & root)
        {
            root.left()->match([&](syncset <uint64_t> :: single & left)
            {
                root.right()->match([&](syncset <uint64_t> :: empty & right)
                {
                    success = (left.element() == 42) && (root.label() == hash(hash(uint64_t(42)), hash()));
                });
            });
        });

        if(!success)
            throw "`single.push()` does not work.";

        (*(root.reinterpret <syncset <uint64_t> :: multiple> ().right())) = root.reinterpret <syncset <uint64_t> :: multiple> ().right()->reinterpret <syncset <uint64_t> :: empty> ().fill(13);
        root.reinterpret <syncset <uint64_t> :: multiple> ().refresh();

        if(root.reinterpret <syncset <uint64_t> :: multiple> ().label() != hash(hash(uint64_t(42)), hash(uint64_t(13))) || root.reinterpret <syncset <uint64_t> :: multiple> ().size() != 2)
            throw "`empty.fill()` on right child does not work.";


        uint64_t total = 0;
        root.reinterpret <syncset <uint64_t> :: multiple> ().traverse([&](const uint64_t & value)
        {
            total += value;
        });

        if(total != (13 + 42))
            throw "Something wrong on `traverse` of two elements.";


        (*(root.reinterpret <syncset <uint64_t> :: multiple> ().left())) = root.reinterpret <syncset <uint64_t> :: multiple> ().left()->reinterpret <syncset <uint64_t> :: single> ().empty();
        root.reinterpret <syncset <uint64_t> :: multiple> ().refresh();

        if(root.reinterpret <syncset <uint64_t> :: multiple> ().label() != hash(hash(), hash(uint64_t(13))) || root.reinterpret <syncset <uint64_t> :: multiple> ().size() != 1)
            throw "`single.empty()` on left child does not work.";

        total = 0;
        root.reinterpret <syncset <uint64_t> :: multiple> ().traverse([&](const uint64_t & value)
        {
            total += value;
        });

        if(total != 13)
            throw "Something wrong on `traverse` of one element.";

        auto otherroot = root;

        success = false;
        root.match([&](syncset <uint64_t> :: multiple & root)
        {
            otherroot.match([&](syncset <uint64_t> :: multiple & otherroot)
            {
                if(root.label() == otherroot.label() && root.size() == otherroot.size() && root.left() != otherroot.left() && root.right() != otherroot.right())
                    success = true;
            });
        });

        if(!success)
            throw "Deep copy on `multiple` does not work.";

        if(!root.reinterpret <syncset <uint64_t> :: multiple> ().pullable())
            throw "Something wrong on `pullable`.";

        root = root.reinterpret <syncset <uint64_t> :: multiple> ().pull();

        success = false;
        root.match([&](syncset <uint64_t> :: single & root)
        {
            success = (root.element() == 13);
        });

        if(!success)
            throw "Something wrong on `pull`";
    });

    $test("syncset/add-find", []
    {
        syncset <uint64_t> syncset;
        std :: unordered_set <uint64_t> set;

        for(uint64_t n = 0; n < 1024; n++)
        {
            if(rand() % 2)
            {
                syncset.add(n);
                set.insert(n);
            }
        }

        for(uint64_t n = 0; n < 2048; n++)
        {
            bool shouldfind = (set.find(n) != set.end());
            bool found = syncset.find(n);

            if(shouldfind != found)
                throw "Something went horribly wrong!";
        }
    });

    $test("syncset/remove-find", []
    {
        syncset <uint64_t> syncset;
        std :: unordered_set <uint64_t> set;

        for(uint64_t n = 0; n < 1024; n++)
        {
            syncset.add(n);
            set.insert(n);
        }

        for(uint64_t n = 0; n < 1024; n++)
        {
            if(rand() % 2)
            {
                syncset.remove(n);
                set.erase(n);
            }
        }

        for(uint64_t n = 0; n < 2048; n++)
        {
            bool shouldfind = (set.find(n) != set.end());
            bool found = syncset.find(n);

            if(shouldfind != found)
                throw "Something went horribly wrong!";
        }
    });
};
