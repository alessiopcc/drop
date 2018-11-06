#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <type_traits>

// Includes

#include "bytewise/bytewise.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class myotherclass
    {
    public:

        // Bytewise interface

        template <typename vtype> void accept(bytewise :: reader <vtype> & reader) const
        {
            for(int32_t j = 0; j < 5; j++)
                reader.visit(j);
        }

        template <typename vtype> void accept(bytewise :: writer <vtype> &)
        {
        }
    };

    class myclass
    {
        // Members

        int i = 9;
        double j = 9.99;
        char k = 'q';
        myotherclass m;

        std :: array <uint8_t, 16> q;
        std :: array <std :: vector <std :: array <int32_t, 33>>, 2> h;

        // Bytewise

        $bytewise(i);
        $bytewise(k);
        $bytewise(m);
        $bytewise(q);
        $bytewise(h);
    };

    class myotherfixedclass
    {
        // Members

        std :: array <int, 99> a;

        // Bytewise

        $bytewise(a);
    };

    class myfixedclass
    {
        // Members

        int i;
        char k;
        std :: array <myotherfixedclass, 4> q;

        // Bytewise

        $bytewise(i);
        $bytewise(k);
        $bytewise(q);
    };

    class empty
    {
    };

    class nearlyempty
    {
        // Members

        empty e;

        // Bytewise

        $bytewise(e);
    };

    class myreader
    {
    public:

        bool update(const uint8_t *, size_t)
        {
            return false;
        }
    };

    class mywriter
    {
    public:

        uint8_t * pop(size_t)
        {
            return nullptr;
        }
    };

    // Tests

    $test("bytewise/constraints", []
    {
        if(!(bytewise :: constraints :: readable <myotherclass, myreader> ()))
            throw "`myotherclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myotherclass, mywriter> ()))
            throw "`myotherclass` is not `mywriter` writable.";

        if(!(bytewise :: constraints :: readable <myclass, myreader> ()))
            throw "`myclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myclass, mywriter> ()))
            throw "`myclass` is not `mywriter` writable.";

        if(!(bytewise :: constraints :: readable <myotherfixedclass, myreader> ()))
            throw "`myotherfixedclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myotherfixedclass, mywriter> ()))
            throw "`myotherfixedclass` is not `mywriter` writable.";

        if(!(bytewise :: constraints :: readable <myfixedclass, myreader> ()))
            throw "`myfixedclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myfixedclass, mywriter> ()))
            throw "`myfixedclass` is not `mywriter` writable.";

        if(bytewise :: constraints :: fixed <myotherclass> ())
            throw "`myotherclass` is fixed.";

        if(bytewise :: constraints :: fixed <myclass> ())
            throw "`myotherclass` is fixed.";

        if(!(bytewise :: constraints :: fixed <myotherfixedclass> ()))
            throw "`myotherfixedclass` not is fixed.";

        if(!(bytewise :: constraints :: fixed <myfixedclass> ()))
            throw "`myfixedclass` is not fixed.";

        if(!(bytewise :: constraints :: fixed <int> ()))
            throw "`int` is not fixed.";

        if(bytewise :: constraints :: fixed <empty> ())
            throw "`empty` is fixed.";

        if(bytewise :: constraints :: readable <empty, myreader> ())
            throw "`empty` is `myreader` readable";

        if(bytewise :: constraints :: writable <empty, mywriter> ())
            throw "`empty` is `mywriter` writable";

        if(bytewise :: constraints :: fixed <nearlyempty> ())
            throw "`nearlyempty` is fixed.";

        if(bytewise :: constraints :: readable <nearlyempty, myreader> ())
            throw "`nearlyempty` is `myreader` readable";

        if(bytewise :: constraints :: writable <nearlyempty, mywriter> ())
            throw "`nearlyempty` is `mywriter` writable";

        if(!(bytewise :: constraints :: reader <myreader> ()))
            throw "`myreader` is not recognized as a reader.";

        if(bytewise :: constraints :: writer <myreader> ())
            throw "`myreader` is recognized as a writer.";

        if(!(bytewise :: constraints :: writer <mywriter> ()))
            throw "`mywriter` is not recognized as a writer.";

        if(bytewise :: constraints :: reader <mywriter> ())
            throw "`mywriter` is recognized as a reader.";
    });

    $test("bytewise/traits", []
    {
        if(bytewise :: traits :: size <myotherclass> () != 0)
            throw "`myotherclass` has non-zero `bytewise` size.";

        if(bytewise :: traits :: size <myclass> () != 0)
            throw "`myclass` has non-zero `bytewise` size.";

        if(bytewise :: traits :: size <uint32_t> () != 4)
            throw "The `bytewise` size of `uint32_t` is not 4.'";

        if(bytewise :: traits :: size <std :: array <std :: array <uint64_t, 4>, 13>> () != 416)
            throw "The `bytewise` size of `std :: array <std :: array <uint64_t, 4>, 13>` is not 416.";

        if(bytewise :: traits :: size <myotherfixedclass> () != 396)
            throw "The `bytewise` size of `myotherfixedclass` is not 396.";

        if(bytewise :: traits :: size <myfixedclass> () != 1589)
            throw "The `bytewise` size of `myclass` is not 1589.";
    });

    $test("bytewise/develop", []
    {
        myclass item;
        myreader reader;

        bytewise :: read(reader, item);
    });
};
