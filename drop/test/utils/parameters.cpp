#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "utils/parameters.hpp"
#include "data/variant.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    template <typename... types> class pack
    {
    };

    // Tests

    $test("utils/parameters-concat", []
    {
        if(!std :: is_same <parameters :: concat_t <variant <int>, variant<double>>, variant <int, double>> :: value)
            throw "`concat_t` does not concat `variant <int>` and `variant <double>` into `variant <int, double>`.";

        if(!std :: is_same <parameters :: concat_t <pack <int>, pack<double>>, pack <int, double>> :: value)
            throw "`concat_t` does not concat `pack <int>` and `pack <double>` into `pack <int, double>`.";
    });

    $test("utils/parameters-reverse", []
    {
        if(!std :: is_same <parameters :: reverse_t <variant <int, double, char>>, variant <char, double, int>> :: value)
            throw "`reverse_t` does not reverse types of `variant <int, double, char>` correctly.";

        if(!std :: is_same <parameters :: reverse_t <pack <int, double, char>>, pack <char, double, int>> :: value)
            throw "`reverse_t` does not reverse types of `pack <int, double, char>` correctly.";

        if(!std :: is_same <parameters :: reverse_t <pack<>>, pack<>> :: value)
            throw "`reverse_t` of `pack <>` is not equal to `pack <>`.";

    });

    $test("utils/parameters-frontstrip", []
    {
        if(!std :: is_same <parameters :: frontstrip_t <variant <int, double, char>>, variant <double, char>> :: value)
            throw "`frontstrip_t` does not remove first type from `variant <int, double, char>`.";

        if(!std :: is_same <parameters :: frontstrip_t <pack <int, double, char>>, pack <double, char>> :: value)
            throw "`frontstrip_t` does not remove first type from `pack <int, double, char>`.";
    });

    $test("utils/parameters-backstrip", []
    {
        if(!std :: is_same <parameters :: backstrip_t <variant <int, double, char>>, variant <int, double>> :: value)
            throw "`frontstrip_t` does not remove last type from `variant <int, double, char>`.";

        if(!std :: is_same <parameters :: backstrip_t <pack <int, double, char>>, pack <int, double>> :: value)
            throw "`frontstrip_t` does not remove last type from `pack <int, double, char>`.";
    });

    $test("utils/parameters-repeat", []
    {
        int i = 4;

        parameters :: repeat <4> (i, [](int & a, int & b, int & c, int & d)
        {
            a *= 2;
            b *= 3;
            c++;
            d += 4;
        });

        if(i != 29)
            throw "`repeat` does not provide references to the same object to the callback provided (manipulation test).";

        parameters :: repeat <3> (i, [](const int & a, const int & b, const int & c)
        {
            if((&a != &b) || (&b != &c))
                throw "`repeat` does not provide references to the same object to the callback provided (pointer test).";
        });

        parameters :: repeat <0> (i, []{});
    });
};
