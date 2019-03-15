#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <type_traits>

// Includes

#include "drop/data/variant.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;

    // Classes

    class destructible
    {
    public:

        ~destructible()
        {
            lastop = 'D';
        }
    };

    // Tests

    $test("variant/is", []
    {
        // `variant`

        {
            variant <int, double, char> myvariant;

            if(!myvariant.is <undefined> ())
                throw "Calling `is <undefined>` on undefined variable returns `false`.";
            if(myvariant.is <int> ())
                throw "Calling `is <int>` on undefined variable returns `true`.";
            if(myvariant.is <double> ())
                throw "Calling `is <double>` on undefined variable returns `true`.";
            if(myvariant.is <char> ())
                throw "Calling `is <char>` on undefined variable returns `true`.";

            myvariant = 5;

            if(myvariant.is <undefined> ())
                throw "Calling `is <undefined>` on int variable returns `true`.";
            if(!myvariant.is <int> ())
                throw "Calling `is <int>` on int variable returns `false`.";
            if(myvariant.is <double> ())
                throw "Calling `is <double>` on int variable returns `true`.";
            if(myvariant.is <char> ())
                throw "Calling `is <char>` on int variable returns `true`.";

            myvariant = 5.5;

            if(myvariant.is <undefined> ())
                throw "Calling `is <undefined>` on double variable returns `true`.";
            if(myvariant.is <int> ())
                throw "Calling `is <int>` on double variable returns `true`.";
            if(!myvariant.is <double> ())
                throw "Calling `is <double>` on double variable returns `false`.";
            if(myvariant.is <char> ())
                throw "Calling `is <char>` on double variable returns `true`.";

            myvariant = 'S';

            if(myvariant.is <undefined> ())
                throw "Calling `is <undefined>` on char variable returns `true`.";
            if(myvariant.is <int> ())
                throw "Calling `is <int>` on char variable returns `true`.";
            if(myvariant.is <double> ())
                throw "Calling `is <double>` on char variable returns `true`.";
            if(!myvariant.is <char> ())
                throw "Calling `is <char>` on char variable returns `false`.";
        }

        // `const variant`

        {
            const variant <int, double, char> constundefined;
            const variant <int, double, char> constint = 5;
            const variant <int, double, char> constdouble = 5.5;
            const variant <int, double, char> constchar = 'S';

            if(!constundefined.is <undefined> ())
                throw "Calling `is <undefined>` on undefined variable returns `false`.";
            if(constundefined.is <int> ())
                throw "Calling `is <int>` on undefined variable returns `true`.";
            if(constundefined.is <double> ())
                throw "Calling `is <double>` on undefined variable returns `true`.";
            if(constundefined.is <char> ())
                throw "Calling `is <char>` on undefined variable returns `true`.";

            if(constint.is <undefined> ())
                throw "Calling `is <undefined>` on int variable returns `true`.";
            if(!constint.is <int> ())
                throw "Calling `is <int>` on int variable returns `false`.";
            if(constint.is <double> ())
                throw "Calling `is <double>` on int variable returns `true`.";
            if(constint.is <char> ())
                throw "Calling `is <char>` on int variable returns `true`.";

            if(constdouble.is <undefined> ())
                throw "Calling `is <undefined>` on double variable returns `true`.";
            if(constdouble.is <int> ())
                throw "Calling `is <int>` on double variable returns `true`.";
            if(!constdouble.is <double> ())
                throw "Calling `is <double>` on double variable returns `false`.";
            if(constdouble.is <char> ())
                throw "Calling `is <char>` on double variable returns `true`.";

            if(constchar.is <undefined> ())
                throw "Calling `is <undefined>` on char variable returns `true`.";
            if(constchar.is <int> ())
                throw "Calling `is <int>` on char variable returns `true`.";
            if(constchar.is <double> ())
                throw "Calling `is <double>` on char variable returns `true`.";
            if(!constchar.is <char> ())
                throw "Calling `is <char>` on char variable returns `false`.";
        }
    });

    $test("variant/get", []
    {
        // `variant`

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

            {
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
            }
        }

        // `const variant`

        {
            const variant <int, float, char> myvariant = 'c';
            auto get = myvariant.get <char> ();

            if(get != 'c')
                throw "`myvariant.get <char> ()` on a const variant does not get the correct value.";

            {
                bool except = false;

                try
                {
                    myvariant.get <float> ();
                }
                catch(exception <bad_access, type_mismatch> &)
                {
                    except = true;
                }

                if(!except)
                    throw "`myvariant.get <float> ()` does not raise an exception.";
            }
        }
    });

    $test("variant/reinterpret", []
    {
        // `variant`

        {
            variant <int, char> myvariant = 3;

            if(myvariant.reinterpret <int> () != 3)
                throw "Calling `reinterpret <int>` on a variable storing an int does not return the correct value.";

            myvariant.reinterpret <int> () = 7;

            if(myvariant.reinterpret <int> () != 7)
                throw "Calling `reinterpret <int>` on a variable storing an int does not return the correct value.";
        }

        // `const variant`

        {
            const variant <int, char> myvariant = 3;

            if(myvariant.reinterpret <int> () != 3)
                throw "Calling `reinterpret <int>` on a const variable storing an int does not return the correct value.";
        }
    });

    $test("variant/erase", []
    {
        variant <int> myvariant = 11;
        myvariant.erase();

        if(myvariant)
            throw "`erase` does not set `_typeid` to 0 (i.e. undefined).";

        variant <destructible> erase = destructible();

        lastop = 'X';
        erase.erase();

        if(lastop != 'D')
        {
            std :: cout << lastop << std :: endl;
            throw "`erase` does not call destructor.";
        }
    });
};
