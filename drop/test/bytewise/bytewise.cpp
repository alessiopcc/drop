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

    // Tests

    $test("bytewise/develop", []
    {
        std :: cout << bytewise :: constraints :: fixed <myfixedclass> () << std :: endl;
        std :: cout << bytewise :: constraints :: readable <myfixedclass, void> () << std :: endl;
        std :: cout << bytewise :: constraints :: writable <myfixedclass, void> () << std :: endl;
    });
};
