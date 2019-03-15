#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/concept/expression.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class myclass
    {
    public:

        void g(const double & value)
        {
        }

        const int & q();
    };

    // Tests

    $test("expression/evaluate", []
    {
        if(!($expression($type(myclass).g($type(double)))))
            throw "Expression `myclass.g(double)` evaluates to `false`.";

        if(!($expression($type(myclass).g($type(int)))))
            throw "Expression `myclass.g(int)` evaluates to `false`.";

        if(!($expression($type(myclass).g($type(const int &)))))
            throw "Expression `myclass.g(const int &)` evaluates to `false`.";

        if($expression($type(myclass).g($type(const char *))))
            throw "Expression `myclass.g(const char *)` evaluates to `true`.";

        if($expression($type(myclass).f()))
            throw "Expression `myclass.f()` evaluates to `true`";
    });

    $test("expression/is", []
    {
        if(!($expression($type(myclass).q()).is <const int &> ()))
            throw "Expression `myclass.q()` is not detected to be a `const int &`.";

        if($expression($type(int).f()).is <void> ())
            throw "Expression `int.f()` is detected to be a `void`.";

        if($expression($type(myclass).q()).is <int> ())
            throw "Expression `myclass.q()` is detected to be an `int`.";
    });

    $test("expression/casts", []
    {
        if(!($expression($type(myclass).q()).casts <const int &> ()))
            throw "Expression `myclass.q()` is not detected to cast to `const int &`.";

        if($expression($type(int).f()).casts <void> ())
            throw "Expression `int.f()` is detected to cast to `void`.";

        if(!($expression($type(myclass).q()).casts <int> ()))
            throw "Expression `myclass.q()` is not detected to cast to `int`.";
    });
};
