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

    class destructible
    {
    public:

        // Destructor

        ~destructible()
        {
            lastop = 'D';
        }
    };

    class argful
    {
    public:

        // Members

        double i;

        // Constructors

        argful(const char *, double i, const int &) : i(i)
        {
            lastop = 'P';
        }

        // Destructor

        ~argful()
        {
            lastop = 'D';
        }
    };

    class bytewiseconstructible
    {
    public:

        // Members

        int32_t i;

        // Bytewise

        $bytewise(i);

        // Constructors

        bytewiseconstructible() = default;

        bytewiseconstructible(bytewise)
        {
            lastop = 'B';
        }

        bytewiseconstructible(const int32_t & i) : i(i)
        {
        }
    };

    class defaultconstructible
    {
    public:

        // Members

        int32_t i;
        int32_t j;

        // Bytewise

        $bytewise(i);
        $bytewise(j);

        // Constructors

        defaultconstructible()
        {
            lastop = 'C';
        }

        defaultconstructible(const int32_t & i, const int32_t & j) : i(i), j(j)
        {
        }
    };

    class writer
    {
        // Members

        std :: vector <uint8_t> & data;
        size_t cursor;

    public:

        // Constructors

        writer(std :: vector <uint8_t> & data) : data(data), cursor(0)
        {
        }

        // Methods

        uint8_t * pop(const size_t & size)
        {
            size_t cursor = this->cursor;
            this->cursor += size;

            return this->data.data() + cursor;
        }
    };

    // Tests

    $test("variant/destructor", []
    {
        lastop = 'X';
        {
            variant <destructible> myvariant = destructible();
        }
        if(lastop != 'D')
            throw "When deleted, `variant <destructible>` does not call `destructible`'s destructor.'";
    });

    $test("variant/emplace", []
    {
        variant <argful, int> myvariant;

        {
            lastop = 'X';
            myvariant.emplace <argful> ("rain", 4, 99);

            if(!myvariant.is <argful> ())
                throw "`emplace` method does not initialize variant to `argful`.";
            if(myvariant.reinterpret <argful> ().i != 4.)
                throw "`emplace` method does not correctly set the value.";
            if(lastop != 'P')
                throw "`emplace` method does not call constructor.";
        }

        {
            lastop = 'X';
            myvariant.emplace <int> (33);

            if(lastop != 'D')
                throw "`emplace` method does not call the destructor on the old value.";
            if(!myvariant.is <int> ())
                throw "`emplace` method does not update variant to `int`.";
            if(myvariant.reinterpret <int> () != 33)
                throw "`emplace` method does not correctly update the value.";
        }
    });

    $test("variant/construct", []
    {
        lastop = 'X';
        auto myvariant = variant <argful, int> :: construct <argful> ("ciao", 4, 99);

        if(!myvariant.is <argful> ())
            throw "`construct` static method does not initialize variant to `argful`.";
        if(myvariant.reinterpret <argful> ().i != 4.)
            throw "`construct` static method does not correctly set the value.";
        if(lastop != 'P')
            throw "`construct` static method does not call constructor.";
    });

    $test("variant/bytewise", []
    {
        variant <bytewiseconstructible, defaultconstructible> myvariant = bytewiseconstructible(77);
        auto data = bytewise :: serialize(myvariant);

        if(!(std :: is_same <decltype(data), std :: vector <uint8_t>> :: value))
            throw "`variant <bytewiseconstructible, defaultconstructible>` appears to be fixed.";
        if(data != std :: vector <uint8_t> {1, 77, 0, 0, 0})
            throw "`variant <bytewiseconstructible, defaultconstructible>` is not serialized correctly when `bytewiseconstructible`.";

        myvariant.reinterpret <bytewiseconstructible> ().i = 99;

        {
            lastop = 'X';
            writer mywriter(data);
            bytewise :: write(mywriter, myvariant);
        }

        if(lastop != 'X')
            throw "Writing on `variant <bytewiseconstructible, defaultconstructible>` with matching typeid calls a constructor.";
        if(myvariant.reinterpret <bytewiseconstructible> ().i != 77)
            throw "Writing on `variant <bytewiseconstructible, defaultconstructible>` failed.";

        variant <bytewiseconstructible, defaultconstructible> myothervariant = defaultconstructible(11, 22);
        auto otherdata = bytewise :: serialize(myothervariant);

        if(otherdata != std :: vector <uint8_t> {2, 11, 0, 0, 0, 22, 0, 0, 0})
            throw "`variant <bytewiseconstructible, defaultconstructible>` is not serialized correctly when `defaultconstructible`.";

        {
            lastop = 'X';
            writer mywriter(otherdata);
            bytewise :: write(mywriter, myvariant);
        }

        if(lastop != 'C')
            throw "Writing `defaultconstructible` on `bytewiseconstructible` `variant <bytewiseconstructible, defaultconstructible>` does not call the default constructor.";
        if(myvariant.get <defaultconstructible> ().i != 11 || myvariant.get <defaultconstructible> ().j != 22)
            throw "`defaultconstructible` is not correctly written on `bytewiseconstructible` `variant <bytewiseconstructible, defaultconstructible>`.";

        {
            lastop = 'X';
            writer mywriter(data);
            bytewise :: write(mywriter, myothervariant);
        }

        if(lastop != 'B')
            throw "Writing `bytewiseconstructible` on `defaultconstructible` `variant <bytewiseconstructible, defaultconstructible>` does not call the bytewise constructor.";
        if(myothervariant.get <bytewiseconstructible> ().i != 77)
            throw "`bytewiseconstructible` is not correctly written on `defaultconstructible` `variant <bytewiseconstructible, defaultconstructible>`.";

        bool thrown = false;
        data = {3};

        try
        {
            writer mywriter(data);
            bytewise :: write(mywriter, myvariant);
        }
        catch(exception <write_failed, malformed_type> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Writing malformed typeid on `variant <bytewiseconstructible, defaultconstructible>` does not yield an exception.";

        data = {0};
        {
            writer mywriter(data);
            bytewise :: write(mywriter, myvariant);
        }

        if(myvariant)
            throw "`variant <bytewiseconstructible, defaultconstructible>` is not erased when writing an `undefined` typeid.";
    });
};
