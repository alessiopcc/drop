#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <type_traits>

// Includes

#include "data/variant.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("variant/match", []
    {
        auto lastop = 'X';

        auto empty = [&](){ lastop = 'E'; };
        auto intspecific = [&](int &){ lastop = 'i'; };
        auto constintspecific = [&](const int &){ lastop = 'I'; };
        auto doublespecific = [&](double &){ lastop = 'd'; };
        auto constdoublespecific = [&](const double &){ lastop = 'D'; };
        auto charspecific = [&](char &){ lastop = 'c'; };
        auto constcharspecific = [&](const char &){ lastop = 'C'; };

        // Not compiling cases

        if(variant <int, char, double> :: constraints :: match <true, decltype(intspecific)> ())
            throw "`const variant <int, char, double>` accepts `[](int &){...}` as match case.";

        if(variant <int, char, double> :: constraints :: match <true, decltype(doublespecific)> ())
            throw "`const variant <int, char, double>` accepts `[](double &){...}` as match case.";

        if(variant <int, char, double> :: constraints :: match <true, decltype(charspecific)> ())
            throw "`const variant <int, char, double>` accepts `[](char &){...}` as match case.";

        if(variant <char, double> :: constraints :: match <true, decltype(constintspecific)> ())
            throw "`const variant <char, double>` accepts `[](const int &){...}` as match case.";

        if(variant <char, double> :: constraints :: match <false, decltype(intspecific)> ())
            throw "`variant <char, double>` accepts `[](int &){...}` as match case.";

        if(variant <char, int> :: constraints :: match <false, decltype(doublespecific)> ())
            throw "`variant <char, int>` accepts `[](double &){...}` as match case.";

        if(variant <char, int> :: constraints :: match <true, int> ())
            throw "`const variant <char, int>` accepts `int` as match case.";

        if(variant <char, int> :: constraints :: match <true, char> ())
            throw "`const variant <char, int>` accepts `char` as match case.";

        if(variant <char, int> :: constraints :: match <false, int> ())
            throw "`variant <char, int>` accepts `int` as match case.";

        if(variant <char, int> :: constraints :: match <false, char> ())
            throw "`variant <char, int>` accepts `char` as match case.";

        // `const variant <int, char, double>`

        {
            lastop = 'X';
            const variant <int, char, double> myvariant;
            myvariant.match(empty, constintspecific, constdoublespecific, constcharspecific);
            if(lastop != 'E')
                throw "Undefined `const variant <int, char, double>` calls something different from '[](){..}' as match case.";
        }

        {
            lastop = 'X';
            const variant <int, char, double> myvariant = 33;
            myvariant.match(empty, constintspecific, constdoublespecific, constcharspecific);
            if(lastop != 'I')
                throw "`const variant <int, char, double>` initialized to `int` calls something different from `[](const int &){}` as match case.";
        }

        {
            lastop = 'X';
            const variant <int, char, double> myvariant = 3.3;
            myvariant.match(empty, constintspecific, constdoublespecific, constcharspecific);
            if(lastop != 'D')
                throw "`const variant <int, char, double>` initialized to `double` calls something different from `[](const double &){}` as match case.";
        }

        {
            lastop = 'X';
            const variant <int, char, double> myvariant = 'q';
            myvariant.match(empty, constintspecific, constdoublespecific, constcharspecific);
            if(lastop != 'C')
                throw "`const variant <int, char, double>` initialized to `char` calls something different from `[](const char &){}` as match case.";
        }

        // `variant <int, char, double>`

        {
            lastop = 'X';
            variant <int, char, double> myvariant;
            myvariant.match(empty, intspecific, constintspecific, doublespecific, constdoublespecific, charspecific, constcharspecific);
            if(lastop != 'E')
                throw "Undefined `variant <int, char, double>` calls something different from `[](){}` as match case.";
        }

        {
            lastop = 'X';
            variant <int, char, double> myvariant = 33;

            myvariant.match(empty, intspecific, constintspecific, doublespecific, constdoublespecific, charspecific, constcharspecific);
            if(lastop != 'i')
                throw "`variant <int, char, double>` initialized to `int` does not call first specific match case.";

            myvariant.match(empty, constintspecific, intspecific, doublespecific, constdoublespecific, charspecific, constcharspecific);
            if(lastop != 'I')
                throw "`variant <int, char, double>` initialized to `int` does not call first specific match case.";
        }

        {
            lastop = 'X';
            variant <int, char, double> myvariant = 3.3;

            myvariant.match(empty, intspecific, constintspecific, doublespecific, constdoublespecific, charspecific, constcharspecific);
            if(lastop != 'd')
                throw "`variant <int, char, double>` initialized to `double` does not call first specific match case.";

            myvariant.match(empty, intspecific, constintspecific, constdoublespecific, doublespecific, charspecific, constcharspecific);
            if(lastop != 'D')
                throw "`variant <int, char, double>` initialized to `double` does not call first specific match case.";
        }

        {
            lastop = 'X';
            variant <int, char, double> myvariant = 'q';

            myvariant.match(empty, intspecific, constintspecific, doublespecific, constdoublespecific, charspecific, constcharspecific);
            if(lastop != 'c')
                throw "`variant <int, char, double>` initialized to `char` does not call first specific match case.";

            myvariant.match(empty, intspecific, constintspecific, doublespecific, constdoublespecific, constcharspecific, charspecific);
            if(lastop != 'C')
                throw "`variant <int, char, double>` initialized to `char` does not call first specific match case.";
        }
    });
};
