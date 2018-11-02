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

    class myclass
    {
        // Members

        int i = 9;
        double j = 9.99;
        char k = 'q';

        // Bytewise

        $bytewise(i);
        $bytewise(k);
    };

    // Tests

    $test("bytewise/develop", []
    {
        myclass m;
        introspection :: visit <bytewise> (m, [](auto & member)
        {
            std :: cout << member << std :: endl;
        });
    });
};
