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

    $test("shorthash", []
    {
        {
            shorthash shorthash;

            if(shorthash(44) != shorthash(44))
                throw "Hashing the same value with the same `shorthash` gives different outputs.";

            auto myhash = shorthash(std :: string("extremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstring"));

            auto myotherhash = shorthash(std :: string("extremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstringextremelylongstring"));

            if(myhash != myotherhash)
                throw "Hashing the same extremly long string with the same `shorthash` gives different outputs.";

            if(shorthash(myclass()) != shorthash(myclass()))
                throw "Hashing the same object with the same `shorthash` gives different outputs";
        }

        {
            shorthash myshorthash;
            shorthash myothershorthash;

            if(myshorthash(33) == myothershorthash(33))
                throw "Hashing the same value with different `shorthash` gives the same output.";

            if(myshorthash(std :: string("rain")) == myothershorthash(std :: string("rain")))
                throw "Hashing the same string with different `shorthash` gives the same output.";

            if(myshorthash(myclass()) == myothershorthash(myclass()))
                throw "Hashing the same object with different `shorthash` gives the same output.";
        }

        {
            shorthash shorthash;

            if(shorthash(3) == shorthash(33))
                throw "Hashing the different values with the same `shorthash` gives the same output.";

            if(shorthash(std :: string("rain")) == shorthash(std :: string("Rain")))
                throw "Hashing different string with the same `shorthash` gives the same output.";
        }
    });
};
