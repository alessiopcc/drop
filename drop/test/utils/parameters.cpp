#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "utils/parameters.hpp"
#include "data/variant.hpp"
#include "concept/expression.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    template <typename... types> class pack
    {
    };

    // Tests

    $test("utils/parameters-in", []
    {
        if(parameters :: in <int, pack <>> :: value)
            throw "`in` finds an `int` in `pack <>`.";

        if(parameters :: in <int, pack <double, char, float>> :: value)
            throw "`in` finds an `int` in `pack <double, char, float>`.";

        if(!(parameters :: in <int, pack <double, char, int, float>> :: value))
            throw "`in` does not find an `int` in `pack <double, char, int, float>`.";
    });

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
        {
            auto mylambda = [](int & a, int & b){};
            auto myotherlambda = [](int & a, double & b){};

            if(!parameters :: constraints :: repeat <2, int &, decltype(mylambda)> ())
                throw "`parameters :: constraints :: repeat` returns false with valid lambda and types.";
            if(parameters :: constraints :: repeat <2, char &, decltype(mylambda)> ())
                throw "`parameters :: constraints :: repeat` returns true with invalid types.";
            if(parameters :: constraints :: repeat <2, int &, decltype(myotherlambda)> ())
                throw "`parameters :: constraints :: repeat` returns true with invalid lambda.";
        }

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
        }
    });
};
