#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "concept/expression.hpp"

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
            std :: cout << "Called with value " << value << std :: endl;
        }
    };

    // Tests

    $test("expression/evaluate", []
    {
        if(!($expression($type(myclass).g($type(double)))))
            throw "Expression `myclass.g(double)` evaluates to `false`";

        if(!($expression($type(myclass).g($type(int)))))
            throw "Expression `myclass.g(int)` evaluates to `false`";

        if(!($expression($type(myclass).g($type(const int &)))))
            throw "Expression `myclass.g(const int &)` evaluates to `false`";

        if($expression($type(myclass).g($type(const char *))))
            throw "Expression `myclass.g(const char *)` evaluates to `true`";

        if($expression($type(myclass).f()))
            throw "Expression `myclass.f()` evaluates to `true`";
    });
};
