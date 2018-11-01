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

    class destructible
    {
    public:

        ~destructible()
        {
            lastop = 'D';
        }
    };

    class argful
    {
    public:

        double i;

        argful(const char *, double i, const int &) : i(i)
        {
            lastop = 'P';
        }

        ~argful()
        {
            lastop = 'D';
        }
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

    $test("variant/copy-move", []
    {
        {
            variant <movable> myvariant = movable();
            lastop = 'X';
            variant <movable> myothervariant = std :: move(myvariant);
            if(lastop != 'M')
                throw "Move constructor of `variant <movable>` does not call `movable`'s move constructor.'";
        }

        {
            variant <movable, enablers :: move_constructible <false>> myvariant = movable();
            lastop = 'X';
            variant <movable, enablers :: move_constructible <false>> myothervariant = std :: move(myvariant);
            if(lastop != 'C')
                throw "Move constructor of `variant <movable, enablers :: move_constructible <false>>` does not call `movable`'s copy constructor.'";
        }

        {
            variant <copyable> myvariant = copyable();
            lastop = 'X';
            variant <copyable> myothervariant = myvariant;
            if(lastop != 'C')
                throw "Copy constructor of `variant <copyable>` does not call `copyable`'s copy constructor.'";
        }

        if(std :: is_copy_constructible <variant <copyable, enablers :: copy_constructible <false>>> :: value)
            throw "`variant <copyable, enablers :: copy_constructible <false>>` is copy constructible";
    });

    $test("variant/destructor", []
    {
        lastop = 'X';
        {
            variant <destructible> myvariant = destructible();
        }
        if(lastop != 'D')
            throw "When deleted, `variant <destructible>` does not call `destructible`'s destructor.'";
    });

    $test("variant/is", []
    {
    });

    $test("variant/emplace", []
    {
        variant <argful, int> myvariant;

        bool ok = false;
        lastop = 'X';

        myvariant.emplace <argful> ("ciao", 4, 99);
        myvariant.match([&](const argful & value)
        {
            ok = (value.i == 4.);
        });

        if(!ok || lastop != 'P')
            throw "`emplace` method does not correctly set the value.";

        lastop = 'X';
        myvariant.emplace <int> (33);

        if(lastop != 'D')
            throw "`emplace` method does not call the destructor on the old value.";
    });

    $test("variant/construct", []
    {
        bool ok = false;
        lastop = 'X';

        auto myvariant = variant <argful, int> :: construct <argful> ("ciao", 4, 99);

        myvariant.match([&](const argful & value)
        {
            ok = (value.i == 4.);
        });

        if(!ok || lastop != 'P')
            throw "`construct` static method does not correctly set the value.";
    });

    $test("variant/match", []
    {
    });

    $test("variant/reinterpret", []
    {
    });

    $test("variant/get", []
    {
        variant <int, float, char> myvariant = 'c';

        try
        {
            myvariant.get <char> () = 'q';
        }
        catch(...)
        {
            throw "`myvariant.get <char> ()` raises an exception.";
        }

        if(myvariant.get <char> () != 'q')
            throw "`myvariant.get <char> ()` does not retrieve or set the value appropriately.";

        bool except = false;

        try
        {
            myvariant.get <float> () = 4.44;
        }
        catch(exception <bad_access, type_mismatch> &)
        {
            except = true;
        }

        if(!except)
            throw "`myvariant.get <float> ()` does not raise an exception.";
    });

    $test("variant/assignment", []
    {
        variant <char, double, int> myvariant;
        myvariant = 4;

        if(!(myvariant.is <int> ()))
            throw "Instance assignment operator fails to set the correct type.";

        if(myvariant.reinterpret <int> () != 4)
            throw "Instance assignment operator fails to set the correct value.";

        myvariant = 5;

        if(!(myvariant.is <int> ()))
            throw "Instance assignment operator fails to keep the correct type.";

        if(myvariant.reinterpret <int> () != 5)
            throw "Instance assignment operator fails to update the correct value.";

        myvariant = 4.44;

        if(!(myvariant.is <double> ()))
            throw "Instance assignment operator fails to change to the correct type.";

        if(myvariant.reinterpret <double> () != 4.44)
            throw "Instance assignment operator fails to update the correct value when changing type.";
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
