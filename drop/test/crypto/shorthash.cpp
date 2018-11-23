#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/shorthash.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class myclass
    {
    public:

        // Bytewise

        template <typename vtype> void accept(bytewise :: reader <vtype> & reader) const
        {
            for(uint32_t i = 0; i < 1024; i++)
                reader.visit(i);
        }
    };

    // Tests

    $test("shorthash/develop", []
    {
        shorthash shorthash;

        std :: cout << shorthash(44) << std :: endl;
        std :: cout << shorthash(44) << std :: endl;

        std :: cout << shorthash(std :: string("extremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstring")) << std :: endl;

        std :: cout << shorthash(std :: string("extremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstring")) << std :: endl;

        std :: cout << shorthash(myclass()) << std :: endl;
        std :: cout << shorthash(myclass()) << std :: endl;
    });
};
