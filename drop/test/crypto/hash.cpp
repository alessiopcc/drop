#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/hash.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class myotherclass
    {
    public:

        // Bytewise

        template <typename vtype> void accept(bytewise :: reader <vtype> & reader) const
        {
            for(int32_t j = 0; j < 5; j++)
                reader.visit(j);
        }
    };

    class myclass
    {
    public:

        // Members

        int32_t i = 9;
        double j = 9.99;
        int8_t k = 'q';
        myotherclass m;

        std :: array <uint8_t, 16> q = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        std :: array <std :: vector <std :: array <int32_t, 4>>, 2> h =
        {
            std :: vector <std :: array <int32_t, 4>> {
                {1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12}
            },
            std :: vector <std :: array <int32_t, 4>> {
                {13, 14, 15, 16},
                {17, 18, 19, 20}
            }
        };

        std :: string s = "RAIN rules!";

        // Bytewise

        $bytewise(i);
        $bytewise(k);
        $bytewise(m);
        $bytewise(q);
        $bytewise(h);
        $bytewise(s);

        // Constructors

        myclass()
        {
        }

        myclass(bytewise) : i(0), k(0), q{}, h{}, s("")
        {
        }
    };

    // Tests

    $test("hash/key", []
    {
        if(hash :: key :: random() == hash :: key :: random())
            throw "Two consecutive calls to `hash :: key :: random` generate the same key.";
    });

    $test("hash/state", []
    {
        {
            auto mystate = hash :: state();
            auto myotherstate = hash :: state();
            if(mystate.finalize() != myotherstate.finalize())
                throw "Calling `finalize` on two different default constructed `state` gives different outputs.";
        }

        {
            auto mystate = hash :: state();
            auto myotherstate = hash :: state();
            myclass myclass;

            mystate.update(5);
            mystate.update('r');
            mystate.update(std :: vector <int> {1, 2, 3, 4}, 4);
            mystate.update(myclass);

            myotherstate.update(5);
            myotherstate.update('r');
            myotherstate.update(std :: vector <int> {1, 2, 3, 4}, 4);
            myotherstate.update(myclass);

            if(mystate.finalize() != myotherstate.finalize())
                throw "Hashing the same inputs without a key gives different outputs.";
        }

        {
            auto mystate = hash :: state();
            auto myotherstate = hash :: state();

            mystate.update(5);
            myotherstate.update(7);

            if(mystate.finalize() == myotherstate.finalize())
                throw "Hashing two different inputs without a key gives the same outputs.";
        }

        {
            auto mykey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(mykey);

            if(mystate.finalize() != myotherstate.finalize())
                throw "Calling `finalize` on two different `state` constructed with the same key gives different outputs.";
        }

        {
            auto mykey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(mykey);
            myclass myclass;

            mystate.update(5);
            mystate.update('r');
            mystate.update(std :: vector <int> {1, 2, 3, 4}, 4);
            mystate.update(myclass);

            myotherstate.update(5);
            myotherstate.update('r');
            myotherstate.update(std :: vector <int> {1, 2, 3, 4}, 4);
            myotherstate.update(myclass);

            if(mystate.finalize() != myotherstate.finalize())
                throw "Hashing the same inputs with the same key gives different outputs.";
        }

        {
            auto mykey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(mykey);

            mystate.update(5);
            myotherstate.update(7);

            if(mystate.finalize() == myotherstate.finalize())
                throw "Hashing two different inputs with the same key gives the same outputs.";
        }

        {
            auto mykey = hash :: key :: random();
            auto myotherkey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(myotherkey);
            myclass myclass;

            mystate.update(5);
            mystate.update('r');
            mystate.update(std :: vector <int> {1, 2, 3, 4}, 4);
            mystate.update(myclass);

            myotherstate.update(5);
            myotherstate.update('r');
            myotherstate.update(std :: vector <int> {1, 2, 3, 4}, 4);
            myotherstate.update(myclass);

            if(mystate.finalize() == myotherstate.finalize())
                throw "Hashing the same inputs with different keys gives the same output.";
        }

        {
            auto mystate = hash :: state();
            auto myotherstate = hash :: state();
            myclass myclass;

            mystate.update(5, 'r', 'a', 'i', 'n', myclass);
            myotherstate.update(5, 'r', 'a', 'i', 'n', myclass);

            if(mystate.finalize() != myotherstate.finalize())
                throw "Variadic hashing of same inputs without a key gives different outputs.";
        }

        {
            auto mystate = hash :: state();
            auto myotherstate = hash :: state();

            mystate.update(5, 'r', 'a', 'i', 'n');
            myotherstate.update(5, 'n', 'i', 'a', 'r');

            if(mystate.finalize() == myotherstate.finalize())
                throw "Variadic hashing of two different inputs without a key gives the same outputs.";
        }

        {
            auto mykey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(mykey);
            myclass myclass;

            mystate.update(5, 'r', 'a', 'i', 'n', myclass);
            myotherstate.update(5, 'r', 'a', 'i', 'n', myclass);

            if(mystate.finalize() != myotherstate.finalize())
                throw "Variadic hashing of the same inputs with the same key gives different outputs.";
        }

        {
            auto mykey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(mykey);

            mystate.update(5, 'r', 'a', 'i', 'n');
            myotherstate.update(5, 'n', 'i', 'a', 'r');

            if(mystate.finalize() == myotherstate.finalize())
                throw "Variadic hashing of two different inputs with the same key gives the same outputs.";
        }

        {
            auto mykey = hash :: key :: random();
            auto myotherkey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(myotherkey);
            myclass myclass;

            mystate.update(5, 'r', 'a', 'i', 'n', myclass);
            myotherstate.update(5, 'r', 'a', 'i', 'n', myclass);

            if(mystate.finalize() == myotherstate.finalize())
                throw "Variadic hashing of the same inputs with different keys gives the same output.";
        }

        {
            auto mykey = hash :: key :: random();
            auto myotherkey = hash :: key :: random();
            auto mystate = hash :: state(mykey);
            auto myotherstate = hash :: state(myotherkey);
            myclass myclass;

            mystate.update(5, 'r', 'a', 'i', 'n', myclass);
            myotherstate.update(5, 'r', 'a', 'i', 'n', myclass);

            if(mystate.finalize() == myotherstate.finalize())
                throw "Variadic hashing of the same inputs with different keys gives the same output.";
        }
    });

    $test("hash/constructors", []
    {
        {
            myclass myclass;

            auto myhash = hash(5, 'r', 'a', 'i', 'n', myclass);
            auto myotherhash = hash(5, 'r', 'a', 'i', 'n', myclass);

            if(myhash != myotherhash)
                throw "Hash constructor returns different outputs with the same inputs.";
        }

        {
            myclass myclass;

            auto myhash = hash(5, 'r', 'a', 'i', 'n', myclass);
            auto myotherhash = hash(5, 'R', 'a', 'i', 'n', myclass);

            if(myhash == myotherhash)
                throw "Hash constructor returns same outputs with different inputs.";
        }

        {
            myclass myclass;
            auto mykey = hash :: key :: random();

            auto myhash = hash :: keyed(mykey, 5, 'r', 'a', 'i', 'n', myclass);
            auto myotherhash = hash :: keyed(mykey, 5, 'r', 'a', 'i', 'n', myclass);

            if(myhash != myotherhash)
                throw "`keyed` returns different outputs with the same inputs.";
        }

        {
            myclass myclass;
            auto mykey = hash :: key :: random();

            auto myhash = hash :: keyed(mykey, 5, 'r', 'a', 'i', 'n', myclass);
            auto myotherhash = hash :: keyed(mykey, 5, 'R', 'a', 'i', 'n', myclass);

            if(myhash == myotherhash)
                throw "`keyed` returns same outputs with different inputs.";
        }

        {
            myclass myclass;
            auto mykey = hash :: key :: random();
            auto myotherkey = hash :: key :: random();

            auto myhash = hash :: keyed(mykey, 5, 'r', 'a', 'i', 'n', myclass);
            auto myotherhash = hash :: keyed(myotherkey, 5, 'r', 'a', 'i', 'n', myclass);

            if(myhash == myotherhash)
                throw "`keyed` with different keys and same inputs returns thhe same output.";
        }
    });
};
