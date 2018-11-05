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

        // Bytewise

        $bytewise(i);
        $bytewise(k);
        $bytewise(m);
    };

    // Tests

    $test("bytewise/develop", []
    {
        std :: cout << bytewise :: constraints :: readable <myclass, void> () << std :: endl;
        std :: cout << bytewise :: constraints :: writable <myclass, void> () << std :: endl;
    });
};
