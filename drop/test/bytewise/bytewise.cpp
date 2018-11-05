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

        template <typename vtype> void accept(bytewise :: reader <vtype> &) const
        {
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

    // Tests

    $test("bytewise/constraints", []
    {
        if(!(bytewise :: constraints :: readable <myotherclass, void> ()))
            throw "`myotherclass` is not `void` readable.";

        if(!(bytewise :: constraints :: writable <myotherclass, void> ()))
            throw "`myotherclass` is not `void` writable.";

        if(!(bytewise :: constraints :: readable <myclass, void> ()))
            throw "`myclass` is not `void` readable.";

        if(!(bytewise :: constraints :: writable <myclass, void> ()))
            throw "`myclass` is not `void` writable.";

        if(!(bytewise :: constraints :: readable <myotherfixedclass, void> ()))
            throw "`myotherfixedclass` is not `void` readable.";

        if(!(bytewise :: constraints :: writable <myotherfixedclass, void> ()))
            throw "`myotherfixedclass` is not `void` writable.";

        if(!(bytewise :: constraints :: readable <myfixedclass, void> ()))
            throw "`myfixedclass` is not `void` readable.";

        if(!(bytewise :: constraints :: writable <myfixedclass, void> ()))
            throw "`myfixedclass` is not `void` writable.";

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

        if(bytewise :: constraints :: readable <empty, void> ())
            throw "`empty` is `void` readable";

        if(bytewise :: constraints :: writable <empty, void> ())
            throw "`empty` is `void` writable";

        if(bytewise :: constraints :: fixed <nearlyempty> ())
            throw "`nearlyempty` is fixed.";

        if(bytewise :: constraints :: readable <nearlyempty, void> ())
            throw "`nearlyempty` is `void` readable";

        if(bytewise :: constraints :: writable <nearlyempty, void> ())
            throw "`nearlyempty` is `void` writable";
    });
};
