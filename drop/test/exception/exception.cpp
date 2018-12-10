#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "exception/exception.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tags

    class rain;
    class drop;

    // Classes

    class myclass
    {
    public:

        // Methods

        void raise()
        {
            exception <rain, drop> ().raise(this);
        }

        void raise(const char * what)
        {
            exception <rain, drop> ().raise(this, what);
        }

        template <typename dtype> void raise(const dtype & details)
        {
            exception <rain, drop> ().raise(this, details);
        }

        template <typename dtype> void raise(const char * what, const dtype & details)
        {
            exception <rain, drop> ().raise(this, what, details);
        }
    };

    // Tests

    $test("exception/tag", []
    {
        class rain;
        class drop;
        class test;

        auto myexception = exception <rain, drop, test> ();

        try
        {
            try
            {
                myexception.raise();
            }
            catch (exception <rain, drop, test> &)
            {
            }
        }
        catch (exception <> &)
        {
            throw "`catch (exception <rain, drop, test> &)` does not catch `exception <rain, drop, test>`.";
        }

        try
        {
            try
            {
                myexception.raise();
            }
            catch (exception <rain, drop> &)
            {
            }
        }
        catch (exception <> &)
        {
            throw "`catch (exception <rain, drop> &)` does not catch `exception <rain, drop, test>`.";
        }

        try
        {
            try
            {
                myexception.raise();
            }
            catch (exception <rain> &)
            {
            }
        }
        catch (exception <> &)
        {
            throw "`catch (exception <rain> &)` does not catch `exception <rain, drop, test>`.";
        }

        try
        {
            try
            {
                myexception.raise();
            }
            catch (exception <drop> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <drop> &)`";
            }
            catch (exception <test> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <test> &)`";
            }
            catch (exception <drop, rain> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <drop, rain> &)`";
            }
            catch (exception <drop, test> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <drop, test> &)`";
            }
            catch (exception <test, drop> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <test, drop> &)`";
            }
            catch (exception <test, rain> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <test, rain> &)`";
            }
            catch (exception <rain, test> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <rain, test> &)`";
            }
            catch (exception <rain, test, drop> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <rain, tets, drop> &)`";
            }
            catch (exception <test, drop, rain> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <test, drop, rain> &)`";
            }
            catch (exception <test, rain, drop> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <test, rain, drop> &)`";
            }
            catch (exception <drop, test, rain> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <drop, test, rain> &)`";
            }
            catch (exception <drop, rain, test> &)
            {
                throw "`exception <rain, drop, test>` is caught by `catch (exception <drop, rain, test> &)`";
            }
        }
        catch (exception <> &)
        {
        }
    });

    $test("exception/raise", []
    {
        myclass myclass;

        auto myexception = exception <rain> ();
        bool passed = false;

        {
            try
            {
                myexception.raise();
            }
            catch (exception <> & e)
            {

                if(std :: string(e.what()).compare(std :: string("[exception: (anonymous namespace)::rain]")))
                    throw "Default `what()` of `exception <rain>` is not formatted correctly.";
            }

            auto myotherexception = exception <rain, drop> ();
            try
            {
                myotherexception.raise();
            }
            catch (exception <> & e)
            {
                if(std :: string(e.what()).compare(std :: string("[exception: (anonymous namespace)::rain, (anonymous namespace)::drop]")))
                    throw "Default `what()` of `exception <what, test>` is not formatted correctly.";
            }

            try
            {
                myexception.raise("Rain.");
            }
            catch (exception <> & e)
            {
                if(std :: string(e.what()).compare(std :: string("Rain.")))
                    throw "Calling `what()` inside `raise(\"Rain.\")` catch block does not return \"Rain.\".";
            }
        }

        {
            try
            {
                myexception.raise();
            }
            catch (exception <rain> & e)
            {
                try
                {
                    e.source();
                }
                catch (...)
                {
                    throw "Calling `source` throw something although no source was specified.";
                }
            }

            try
            {
                myclass.raise();
            }
            catch (exception <rain> & e)
            {
                try
                {
                    passed = false;
                    e.source();
                }
                catch (const class myclass *)
                {
                    passed = true;
                }
                if(!passed)
                    throw "`source()` not working correctly inside `myclass.raise()` catch block.";
            }
        }

        {
            try
            {
                myexception.raise();
            }
            catch (exception <rain> & e)
            {
                try
                {
                    e.details();
                }
                catch (...)
                {
                    throw "Calling `details` throw something although no details were specified.";
                }
            }

            try
            {
                myexception.raise(3);
            }
            catch (exception <rain> & e)
            {
                passed = false;
                try
                {
                    e.details();
                }
                catch (int & i)
                {
                    passed = (i == 3);
                }
                if(!passed)
                    throw "`details()` not working correctly inside `raise(3)` catch block.";
            }
        }

        {
            try
            {
                myclass.raise("what");
            }
            catch (exception <rain> & e)
            {
                if(std :: string(e.what()).compare(std :: string("what")))
                    throw "Calling `what()` inside `myclass.raise(\"what\")` catch block does not return \"what\".";

                passed = false;
                try
                {
                    e.source();
                }
                catch (const class myclass *)
                {
                    passed = true;
                }
                if(!passed)
                    throw "`source()` not working correctly inside `myclassraise(\"what\")` catch block.";
            }
        }

        {
            passed = false;
            try
            {
                myexception.raise("what", 'd');
            }
            catch (exception <rain> & e)
            {
                if(std :: string(e.what()).compare(std :: string("what")))
                    throw "Calling `what()` inside `raise(\"what\", test)` catch block does not return \"what\".";

                try
                {
                    e.details();
                }
                catch (char & c)
                {
                    passed = (c == 'd');
                }
            }
            if(!passed)
                throw "`details()` not working correctly inside `raise(\"what\", 'd')` catch block.";
        }

        {
            try
            {
                myclass.raise(7);
            }
            catch (exception <rain> & e)
            {
                passed = false;
                try
                {
                    e.source();
                }
                catch (const class myclass *)
                {
                    passed = true;
                }
                if(!passed)
                    throw "`source()` not working correctly inside `myclass.raise(7)` catch block.";

                passed = false;
                try
                {
                    e.details();
                }
                catch (int & i)
                {
                    passed = (i == 7);
                }
                if(!passed)
                    throw "`details` not working correctly inside `myclass.raise(7)` catch block.";
            }
        }

        {
            try
            {
                myclass.raise("what", 11);
            }
            catch (exception <rain> & e)
            {
                if(std :: string(e.what()).compare(std :: string("what")))
                    throw "Calling `what()` inside `myclass.raise(\"what\", 11)` catch block does not return \"what\".";

                passed = false;
                try
                {
                    e.source();
                }
                catch (const class myclass *)
                {
                    passed = true;
                }
                if(!passed)
                    throw "`source()` not working correctly inside `myclass.raise(\"what\", 11)` catch block.";

                passed = false;
                try
                {
                    e.details();
                }
                catch (int & i)
                {
                    passed = (i == 11);
                }
                if(!passed)
                    throw "`details()` not working correctly inside `myclass.raise(\"what\", 11)` catch block.";
            }
        }
    });
};
