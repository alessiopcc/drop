#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/concept/callable.h"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("callable/evaluate", []
    {
        auto clambda = [](const int &)
        {
        };

        auto mlambda = [](const int &) mutable
        {
        };

        if(!(callable <decltype(clambda), const int &> :: value))
            throw "Cannot call `clambda(const int &).`";

        if(!(callable <decltype(clambda) &, const int &> :: value))
            throw "Cannot call `(clambda &)(const int &).`";

        if(!(callable <const decltype(clambda), const int &> :: value))
            throw "Cannot call `(const clambda)(const int &).`";

        if(!(callable <const decltype(clambda) &, const int &> :: value))
            throw "Cannot call `(const clambda &)(const int &).`";

        if(!(callable <decltype(mlambda), const int &> :: value))
            throw "Cannot call `mlambda(const int &).`";

        if(!(callable <decltype(mlambda) &, const int &> :: value))
            throw "Cannot call `(mlambda &)(const int &).`";

        if(callable <const decltype(mlambda), const int &> :: value)
            throw "Can call `(const mlambda)(const int &).`";

        if(callable <const decltype(mlambda) &, const int &> :: value)
            throw "Can call `(const mlambda &)(const int &).`";

        if(callable <decltype(clambda), int &> :: value)
            throw "Can call `clambda(int &).`";

        if(callable <decltype(clambda), int> :: value)
            throw "Can call `clambda(int).`";

        if(callable <decltype(clambda), const int> :: value)
            throw "Can call `clambda(const int).`";

        auto alambda = [](auto &)
        {
        };

        auto calambda = [](const auto &)
        {
        };

        if(!(callable <decltype(alambda), int &> :: value))
            throw "Cannot call `alambda(int &)`.";

        if(!(callable <decltype(alambda), const int &> :: value))
            throw "Cannot call `alambda(const int &)`.";

        if(!(callable <decltype(alambda), double &> :: value))
            throw "Cannot call `alambda(double &)`.";

        if(!(callable <decltype(alambda), const double &> :: value))
            throw "Cannot call `alambda(const double &)`.";

        if(callable <decltype(calambda), int &> :: value)
            throw "Can call `calambda(int &)`.";

        if(!(callable <decltype(calambda), const int &> :: value))
            throw "Cannot call `calambda(const int &)`.";

        if(callable <decltype(calambda), double &> :: value)
            throw "Can call `calambda(double &)`.";

        if(!(callable <decltype(calambda), const double &> :: value))
            throw "Cannot call `calambda(const double &)`.";
    });
};
