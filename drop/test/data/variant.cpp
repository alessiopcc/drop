#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "data/variant.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;

    // Classes

    class movable
    {
    public:

        movable()
        {
        }

        movable(const movable &)
        {
            lastop = 'C';
        }

        movable(movable &&)
        {
            lastop = 'M';
        }
    };

    class copyable
    {
    public:

        copyable()
        {
        }

        copyable(const copyable &)
        {
            lastop = 'C';
        }
    };

    class myclass
    {
    public:

        int i;
    };

    // Tests

    $test("variant/constructors", []
    {
        if(!(variant <int, double, char> ().is <undefined> ()))
            throw "Uninitialized `variant` is not `undefined`.";

        if(!(variant <int, double, char> (44).is <int> ()))
            throw "`variant` that was copy-initialized with `int` is not an `int`.";

        if(!(variant <int, double, float, char> (4.44).is <double> ()))
            throw "`variant` that was copy-initialized with `double` is not a `double`.";

        if(!(variant <int, double, float, char> (4.44f).is <float> ()))
            throw "`variant` that was copy-initialized with `float` is not a `float`.";

        {
            lastop = 'X';
            variant <movable> v(movable{});
            if(lastop != 'M')
                throw "In-place initialization of `variant` with `movable` does not call move constructor.";
        }

        {
            lastop = 'X';
            movable m;
            variant <movable> v(m);
            if(lastop != 'C')
                throw "Copy construction of `variant` with `movable` does not call copy constructor.";
        }

        {
            lastop = 'X';
            variant <copyable> v(copyable{});
            if(lastop != 'C')
                throw "In-place initialization of `variant` with `copyable` does not call copy constructor.";
        }

        {
            lastop = 'X';
            copyable c;
            variant <copyable> v(c);
            if(lastop != 'C')
                throw "Copy construction of `variant` with `copyable` does not call copy constructor.";
        }
    });

    $test("variant/visit", []
    {
        auto empty = [](){};
        auto acceptdouble = [](double){};
        auto acceptanything = [](auto &&){};

        if(variant <int, double, char> :: constraints :: visitor <false, decltype(empty)> ())
            throw "`variant <int, double, char>` can be visited by lambda with no arguments.";

        if(variant <int, double, char> :: constraints :: visitor <true, decltype(empty)> ())
            throw "`const variant <int, double, char>` can be visited by lambda with no arguments.";

        if(!(variant <int, double, char> :: constraints :: visitor <false, decltype(acceptdouble)> ()))
            throw "`variant <int, double, char>` cannot be visited by lambda with `double` argument.";

        if(!(variant <int, double, char> :: constraints :: visitor <true, decltype(acceptdouble)> ()))
            throw "`const variant <int, double, char>` cannot be visited by lambda with `double` argument.";

        if(variant <int, double, char, movable> :: constraints :: visitor <false, decltype(acceptdouble)> ())
            throw "`variant <int, double, char, movable>` can be visited by lambda with `double` argument.";

        if(variant <int, double, char, movable> :: constraints :: visitor <true, decltype(acceptdouble)> ())
            throw "`const variant <int, double, char, movable>` can be visited by lambda with `double` argument.";

        if(!(variant <int, double, char, movable> :: constraints :: visitor <false, decltype(acceptanything)> ()))
            throw "`variant <int, double, char, movable>` cannot be visited by lambda with `auto` argument.";

        if(!(variant <int, double, char, movable> :: constraints :: visitor <true, decltype(acceptanything)> ()))
            throw "`const variant <int, double, char, movable>` cannot be visited by lambda with `auto` argument.";

        {
            variant <int, double, char> v = 'q';
            v.visit([](auto && value)
            {
                if(!(std :: is_same <decltype(value), char &> :: value))
                    throw "`variant` does not apply `char &` to visitor correctly.";
            });
        }

        {
            const variant <int, double, char> v = 'q';
            v.visit([](auto && value)
            {
                if(!(std :: is_same <decltype(value), const char &> :: value))
                    throw "`const variant` does not apply `const char &` to visitor correctly.";
            });
        }
    });

    template <typename type> void print()
    {
        std :: cout << __PRETTY_FUNCTION__ << std :: endl;
    }

    $test("variant/match", []
    {
        auto empty = [](){};
        auto intspecific = [](int &){};
        auto constintspecific = [](const int &){};
        auto doublespecific = [](double &){};
        auto constdoublespecific = [](const double &){};
        auto charspecific = [](char &){};
        auto constcharspecific = [](const char &){};
        auto myclassspecific = [](myclass &){};
        auto constmyclassspecific = [](const myclass &){};

        if(variant <int, char, myclass, double> :: constraints :: match <false, decltype(empty)> ())
            throw "`variant <int, char, myclass, double>` accepts `(empty)` as match cases.";

        bool lastop;

        {
            lastop = false;
            const variant <int, char, myclass, double> myvariant = 'q';

            myvariant.match([&](auto &)
            {
                lastop = true;
            }, [](char &)
            {
            });

            if(!lastop)
                throw "`const variant <int, char, myclass, double>` accepts `[](char &){}` as specific match case.";
        }
    });

    $test("variant/cast", []
    {
        variant <int> emptyvariant;
        variant <int> fullvariant = 33;

        if(emptyvariant)
            throw "Undefined `variant` casts to boolean `true`.";

        if(!fullvariant)
            throw "Non-undefined `variant` casts to boolean `false`.";
    });
};
