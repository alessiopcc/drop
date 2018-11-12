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
    char lastassign;
    bool destroy;

    // Methods

    void reset()
    {
        lastop = 'X';
        lastassign = 'x';
        destroy = false;
    }

    // Classes

    class movable
    {
    public:

        movable()
        {
            lastop = 'I';
        }

        movable(const movable &)
        {
            lastop = 'C';
        }

        movable(movable &&)
        {
            lastop = 'M';
        }

        movable & operator = (const movable &)
        {
            lastassign = 'c';
            return (*this);
        }

        movable & operator = (movable &&)
        {
            lastassign = 'm';
            return (*this);
        }

        ~movable()
        {
            destroy = true;
        }
    };

    class copyable
    {
    public:

        copyable()
        {
            lastop = 'I';
        }

        copyable(const copyable &)
        {
            lastop = 'C';
        }

        copyable & operator = (const copyable &)
        {
            lastassign = 'c';
            return (*this);
        }

        ~copyable()
        {
            destroy = true;
        }
    };

    // Tests

    $test("variant/constructors-basic", []
    {
        if(!(variant <int, double, char> ().is <undefined> ()))
            throw "Uninitialized `variant` is not `undefined`.";
        if(!(variant <int, double, char> (44).is <int> ()))
            throw "`variant` that was copy-initialized with `int` is not an `int`.";
        if(!(variant <int, double, float, char> (4.44).is <double> ()))
            throw "`variant` that was copy-initialized with `double` is not a `double`.";
        if(!(variant <int, double, float, char> (4.44f).is <float> ()))
            throw "`variant` that was copy-initialized with `float` is not a `float`.";

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

    $test("variant/constructors-copy", []
    {
        copyable c;
        movable m;

        {
            reset();
            variant <copyable, movable> x = m;
            if(lastop != 'C')
                throw "Direct initialization by copy construction of `variant` with `movable`, does not call copy constructor.";
            if(destroy)
                throw "Direct initialization by copy construction of `variant` with `movable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by copy construction of `variant` with `movable`, calls assignment operator.";
        }

        {
            reset();
            variant <copyable, movable> x = c;
            if(lastop != 'C')
                throw "Direct initialization by copy construction of `variant` with `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Direct initialization by copy construction of `variant` with `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by copy construction of `variant` with `copyable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x;
            reset();
            x = m;
            if(lastop != 'C')
                throw "Initialization by copy construction of `variant` with `movable`, does not call copy constructor.";
            if(destroy)
                throw "Initialization by copy construction of `variant` with `movable`, calls destructor.";
        }

        {
            variant <copyable, movable> x;
            reset();
            x = c;
            if(lastop != 'C')
                throw "Copy construction of `variant` with `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Copy construction of `variant` with `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Copy construction of `variant` with `copyable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x = c;
            reset();
            x = m;
            if(lastop != 'C')
                throw "Assigning `movable` as new type to `variant` by copy construction, does not call copy constructor.";
            if(!destroy)
                throw "Assigning `movable` as new type to `variant` by copy construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning `movable` as new type to `variant` by copy construction, calls assignment operator.";
        }

        {
            variant <copyable, movable> x = m;
            reset();
            x = c;
            if(lastop != 'C')
                throw "Assigning `copyable` as new type to `variant` by copy construction, does not call copy constructor.";
            if(!destroy)
                throw "Assigning `copyable` as new type to `variant` by copy construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning `copyable` as new type to `variant` by copy construction, calls assignment operator.";
        }
    });

    $test("variant/constructors-move", []
    {
        copyable c;
        movable m;

        {
            reset();
            variant <copyable, movable> x = std :: move(m);
            if(lastop != 'M')
                throw "Direct initialization by move construction of `variant` with `movable`, does not call move constructor.";
            if(destroy)
                throw "Direct initialization by move construction of `variant` with `movable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by move construction of `variant` with `movable`, calls assignment operator.";
        }

        {
            reset();
            variant <copyable, movable> x = std :: move(c);
            if(lastop != 'C')
                throw "Direct initialization by move construction of `variant` with `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Direct initialization by move construction of `variant` with `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by move construction of `variant` with `copyable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x;
            reset();
            x = std :: move(m);
            if(lastop != 'M')
                throw "Initialization by move construction of `variant` with `movable`, does not call move constructor.";
            if(destroy)
                throw "Initialization by move construction of `variant` with `movable`, calls destructor.";
            if(lastassign != 'x')
                throw "Initialization by move construction of `variant` with `movable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x;
            reset();
            x = std :: move(c);
            if(lastop != 'C')
                throw "Initialization by move construction of `variant` with `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Initialization by move construction of `variant` with `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Initialization by move construction of `variant` with `copyable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x = c;
            reset();
            x = std :: move(m);
            if(lastop != 'M')
                throw "Assigning `movable` as new type to `variant`, by move construction, does not call copy constructor.";
            if(!destroy)
                throw "Assigning `movable` as new type to `variant`, by move construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning `movable` as new type to `variant`, by move construction, calls assignment operator.";
        }

        {
            variant <copyable, movable> x = m;
            reset();
            x = std :: move(c);
            if(lastop != 'C')
                throw "Assigning `copyable` as new type to `variant` by move construction, does not call copy constructor.";
            if(!destroy)
                throw "Assigning `copyable` as new type to `variant` by move construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning `copyable` as new type to `variant` by move construction, calls assignment operator.";
        }
    });

    $test("variant/constructors-copyvariant", []
    {
        variant <copyable, movable> c = copyable();
        variant <copyable, movable> m = movable();

        {
            reset();
            variant <copyable, movable> x = c;
            if(lastop != 'C')
                throw "Direct initialization by copy construction of `variant` with a `variant` of type `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Direct initialization by copy construction of `variant` with a `variant` of type `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by copy construction of `variant` with a `variant` of type `copyable`, calls assignment operator.";
        }

        {
            reset();
            variant <copyable, movable> x = m;
            if(lastop != 'C')
                throw "Direct initialization by copy construction of `variant` with a `variant` of type `movable`, does not call copy constructor.";
            if(destroy)
                throw "Direct initialization by copy construction of `variant` with a `variant` of type `movable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by copy construction of `variant` with a `variant` of type `movable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x;
            reset();
            x = c;
            if(lastop != 'C')
                throw "Initialization by copy construction of `variant` with a `variant` of type `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Initialization by copy construction of `variant` with a `variant` of type `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Initialization by copy construction of `variant` with a `variant` of type `copyable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x;
            reset();
            x = m;
            if(lastop != 'C')
                throw "Initialization by copy construction of `variant` with a `variant` of type `movable`, does not call copy constructor.";
            if(destroy)
                throw "Initialization by copy construction of `variant` with a `variant` of type `movable`, call destructor.";
            if(lastassign != 'x')
                throw "Initialization by copy construction of `variant` with a `variant` of type `movable`, call assignment operator.";
        }

        {
            variant <copyable, movable> x = m;
            reset();
            x = c;
            if(lastop != 'C')
                throw "Assigning a `variant` of type `copyable` to `variant` by copy construction, does not call copy constructor.";
            if(!destroy)
                throw "Assigning a `variant` of type `copyable` to `variant` by copy construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning a `variant` of type `copyable` to `variant` by copy construction, calls assignment operator.";
        }

        {
            variant <copyable, movable> x = c;
            reset();
            x = m;
            if(lastop != 'C')
                throw "Assigning a `variant` of type `movable` to `variant` by copy construction, does not call copy constructor.";
            if(!destroy)
                throw "Assigning a `variant` of type `movable` to `variant` by copy construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning a `variant` of type `movable` to `variant` by copy construction, calls assignment operator.";
        }
    });

    $test("variant/constructors-movevariant", []
    {
        variant <copyable, movable> c = copyable();
        variant <copyable, movable> m = movable();

        {
            reset();
            variant <copyable, movable> x = std :: move(c);
            if(lastop != 'C')
                throw "Direct initialization by move construction of `variant` with a `variant` of type `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Direct initialization by move construction of `variant` with a `variant` of type `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by move construction of `variant` with a `variant` of type `copyable`, calls assignment operator.";
        }

        // WTF 1
        {
            lastop = 'X';
            destroy = false;
            variant <copyable, movable> x = std :: move(m);
            if(lastop != 'M')
                throw "Direct initialization by move construction of `variant` with a `variant` of type `movable`, does not call move constructor.";
            if(destroy)
                throw "Direct initialization by move construction of `variant` with a `variant` of type `movable`, calls destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by move construction of `variant` with a `variant` of type `movable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x;
            reset();
            x = std :: move(c);
            if(lastop != 'C')
                throw "Initialization by move construction of `variant` with a `variant` of type `copyable`, does not call copy constructor.";
            if(destroy)
                throw "Initialization by move construction of `variant` with a `variant` of type `copyable`, calls destructor.";
            if(lastassign != 'x')
                throw "Initialization by move construction of `variant` with a `variant` of type `copyable`, calls assignment operator.";
        }

        // WTF 2
        {
            variant <copyable, movable> x;
            reset();
            x = std :: move(m);
            if(lastop != 'M')
                throw "Initialization by move construction of `variant` with a `variant` of type `movable`, does not call move constructor.";
            if(destroy)
                throw "Initialization by move construction of `variant` with a `variant` of type `movable`, calls destructor.";
            if(lastassign != 'x')
                throw "Initialization by move construction of `variant` with a `variant` of type `movable`, calls assignment operator.";
        }

        {
            variant <copyable, movable> x = m;
            reset();
            x = std :: move(c);
            if(lastop != 'C')
                throw "Assigning a `variant` of type `copyable` to `variant` by move construction, does not call copy constructor.";
            if(!destroy)
                throw "Assigning a `variant` of type `copyable` to `variant` by move construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning a `variant` of type `copyable` to `variant` by move construction, calls assignment operator.";
        }

        // WTF 3
        {
            variant <copyable, movable> x = c;
            reset();
            x = std :: move(m);
            if(lastop != 'M')
                throw "Assigning a `variant` of type `movable` to `variant` by move construction, does not call move constructor.";
            if(!destroy)
                throw "Assigning a `variant` of type `movable` to `variant` by move construction, does not call destructor.";
            if(lastassign != 'x')
                throw "Assigning a `variant` of type `movable` to `variant` by move construction, calls assignment operator.";
        }
    });

    $test("variant/constructors-anonymous", []
    {
        {
            reset();
            variant <copyable, movable> x = copyable();
            if(lastop != 'C')
                throw "Direct initialization of `variant` with anonymous `copyable`, does not call copy constructor.";
            if(!destroy)
                throw "Direct initialization of `variant` with anonymous `copyable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Direct initialization of `variant` with anonymous `copyable`, calls assignment operator.";
        }

        {
            reset();
            variant <copyable, movable> x = movable();
            if(lastop != 'M')
                throw "Direct initialization of `variant` with anonymous `movable`, does not call move constructor.";
            if(!destroy)
                throw "Direct initialization of `variant` with anonymous `movable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Direct initialization of `variant` with anonymous `movable`, calls assignment operator.";
        }

        {
            reset();
            variant <copyable, movable> x = std :: move(copyable());
            if(lastop != 'C')
                throw "Initialization by move construction of `variant` with anonymous `copyable`, does not call copy constructor.";
            if(!destroy)
                throw "Initialization by move construction of `variant` with anonymous `copyable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Initialization by move construction of `variant` with anonymous `copyable`, calls assignment operator.";
        }

        {
            reset();
            variant <copyable, movable> x = std :: move(movable());
            if(lastop != 'M')
                throw "Initialization by move construction of `variant` with anonymous `movable`, does not call copy constructor.";
            if(!destroy)
                throw "Initialization by move construction of `variant` with anonymous `movable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Initialization by move construction of `variant` with anonymous `movable`, calls assignment operator.";
        }
    });

    // const

    $test("variant/constructors-copy-const", []
    {
        {
            copyable c;
            movable m;

            {
                reset();
                const variant <copyable, movable> x = m;
                if(lastop != 'C')
                    throw "Direct initialization by copy construction of `const variant` with `movable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by copy construction of `const variant` with `movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by copy construction of `const variant` with `movable`, calls assignment operator.";
            }

            {
                reset();
                const variant <copyable, movable> x = c;
                if(lastop != 'C')
                    throw "Direct initialization by copy construction of `const variant` with `copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by copy construction of `const variant` with `copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by copy construction of `const variant` with `copyable`, calls assignment operator.";
            }
        }

        {
            const copyable c;
            const movable m;

            {
                reset();
                const variant <copyable, movable> x = m;
                if(lastop != 'C')
                    throw "Direct initialization by copy construction of `const variant` with `const movable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by copy construction of `const variant` with `const movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by copy construction of `const variant` with `const movable`, calls assignment operator.";
            }

            {
                reset();
                const variant <copyable, movable> x = c;
                if(lastop != 'C')
                    throw "Direct initialization by copy construction of `const variant` with `const copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by copy construction of `const variant` with `const copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by copy construction of `const variant` with `const copyable`, calls assignment operator.";
            }
        }
    });

    $test("variant/constructors-move-const", []
    {
        {
            copyable c;
            movable m;

            {
                reset();
                const variant <copyable, movable> x = std :: move(m);
                if(lastop != 'M')
                    throw "Direct initialization by move construction of `const variant` with `movable`, does not call move constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with `movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with `movable`, calls assignment operator.";
            }

            {
                reset();
                const variant <copyable, movable> x = std :: move(c);
                if(lastop != 'C')
                    throw "Direct initialization by move construction of `const variant` with `copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with `copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with `copyable`, calls assignment operator.";
            }
        }

        {
            const copyable c;
            const movable m;

            {
                reset();
                const variant <copyable, movable> x = std :: move(m);
                if(lastop != 'C')
                    throw "Direct initialization by move construction of `const variant` with `const movable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with `const movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with `const movable`, calls assignment operator.";
            }

            {
                reset();
                const variant <copyable, movable> x = std :: move(c);
                if(lastop != 'C')
                    throw "Direct initialization by move construction of `const variant` with `const copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with `const copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with `const copyable`, calls assignment operator.";
            }
        }
    });

    $test("variant/constructors-copyvariant-const", []
    {
        {
            variant <copyable, movable> c = copyable();
            variant <copyable, movable> m = movable();

            {
                reset();
                const variant <copyable, movable> x = c;
                if(lastop != 'C')
                    throw "Direct initialization by copy construction of `const variant` with a `variant` of type `copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by copy construction of `const variant` with a `variant` of type `copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by copy construction of `const variant` with a `variant` of type `copyable`, calls assignment operator.";
            }

            {
                reset();
                const variant <copyable, movable> x = m;
                if(lastop != 'C')
                    throw "Direct initialization by copy construction of `const variant` with a `variant` of type `movable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by copy construction of `const variant` with a `variant` of type `movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by copy construction of `const variant` with a `variant` of type `movable`, calls assignment operator.";
            }
        }

        {
            const variant <copyable, movable> c = copyable();
            const variant <copyable, movable> m = movable();

            {
                reset();
                const variant <copyable, movable> x = c;
                if(lastop != 'C')
                    throw "Direct nitialization by copy construction of `const variant` with a `const variant` of type `copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct nitialization by copy construction of `const variant` with a `const variant` of type `copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct nitialization by copy construction of `const variant` with a `const variant` of type `copyable`, calls assignment operator.";
            }

            {
                reset();
                const variant <copyable, movable> x = m;
                if(lastop != 'C')
                    throw "Direct initialization by copy construction of `const variant` with a `const variant` of type `movable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by copy construction of `const variant` with a `const variant` of type `movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by copy construction of `const variant` with a `const variant` of type `movable`, calls assignment operator.";
            }
        }
    });

    $test("variant/constructors-movevariant-const", []
    {
        {
            variant <copyable, movable> c = copyable();
            variant <copyable, movable> m = movable();

            {
                reset();
                const variant <copyable, movable> x = std :: move(c);
                if(lastop != 'C')
                    throw "Direct initialization by move construction of `const variant` with a `variant` of type `copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with a `variant` of type `copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with a `variant` of type `copyable`, calls assignment operator.";
            }

            // WTF 4
            {
                reset();
                const variant <copyable, movable> x = std :: move(m);
                if(lastop != 'M')
                    throw "Direct initialization by move construction of `const variant` with a `variant` of type `movable`, does not call move constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with a `variant` of type `movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with a `variant` of type `movable`, calls assignment operator.";
            }
        }

        {
            const variant <copyable, movable> c = copyable();
            const variant <copyable, movable> m = movable();

            {
                reset();
                const variant <copyable, movable> x = std :: move(c);
                if(lastop != 'C')
                    throw "Direct initialization by move construction of `const variant` with a `const variant` of type `copyable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with a `const variant` of type `copyable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with a `const variant` of type `copyable`, calls assignment operator.";
            }

            {
                reset();
                const variant <copyable, movable> x = std :: move(m);
                if(lastop != 'C')
                    throw "Direct initialization by move construction of `const variant` with a `const variant` of type `movable`, does not call copy constructor.";
                if(destroy)
                    throw "Direct initialization by move construction of `const variant` with a `const variant` of type `movable`, calls destructor.";
                if(lastassign != 'x')
                    throw "Direct initialization by move construction of `const variant` with a `const variant` of type `movable`, calls assignment operator.";
            }
        }
    });

    $test("variant/constructors-anonymous-const", []
    {
        {
            reset();
            const variant <copyable, movable> x = copyable();
            if(lastop != 'C')
                throw "Direct initialization by copy construction of `const variant` with anonymous `copyable`, does not call copy constructor.";
            if(!destroy)
                throw "Direct initialization by copy construction of `const variant` with anonymous `copyable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by copy construction of `const variant` with anonymous `copyable`, calls assignment operator.";
        }

        {
            reset();
            const variant <copyable, movable> x = movable();
            if(lastop != 'M')
                throw "Direct initialization by copy construction of `const variant` with anonymous `movable`, does not call move constructor.";
            if(!destroy)
                throw "Direct initialization by copy construction of `const variant` with anonymous `movable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by copy construction of `const variant` with anonymous `movable`, calls assignment operator.";
        }

        {
            reset();
            const variant <copyable, movable> x = std :: move(copyable());
            if(lastop != 'C')
                throw "Direct initialization by move construction of `const variant` with anonymous `copyable`, does not call copy constructor.";
            if(!destroy)
                throw "Direct initialization by move construction of `const variant` with anonymous `copyable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by move construction of `const variant` with anonymous `copyable`, calls assignment operator.";
        }

        {
            reset();
            const variant <copyable, movable> x = std :: move(movable());
            if(lastop != 'M')
                throw "Direct initialization by copy construction of `const variant` with anonymous `movable`, does not call move constructor.";
            if(!destroy)
                throw "Direct initialization by copy construction of `const variant` with anonymous `movable`, does not call destructor.";
            if(lastassign != 'x')
                throw "Direct initialization by copy construction of `const variant` with anonymous `movable`, calls assignment operator.";
        }
    });

    //--------------------------------------------------------------------------
    // Assignment
    //--------------------------------------------------------------------------

    $test("variant/assignment-copy", []
    {
        copyable c;
        movable m;

        {
            variant <copyable, movable> x = copyable();
            reset();
            x = c;
            if(lastop != 'X')
                throw "Assigning `copayble` by copy construction to `variant`, calls a constructor.";
            if(lastassign != 'c')
                throw "Assigning `copayble` by copy construction to `variant`, does not call copy assignment operator.";
            if(destroy)
                throw "Assigning `copayble` by copy construction to `variant`, calls destructor.";
        }

        {
            variant <copyable, movable> x = movable();
            reset();
            x = m;
            if(lastop != 'X')
                throw "Assigning `movable` by copy construction to `variant`, calls a constructor.";
            if(lastassign != 'c')
                throw "Assigning `movable` by copy construction to `variant`, does not call copy assignment operator.";
            if(destroy)
                throw "Assigning `movable` by copy construction to `variant`, calls destructor.";
        }
    });

    $test("variant/assignment-move", []
    {
        copyable c;
        movable m;

        {
            variant <copyable, movable> x = copyable();
            reset();
            x = std:: move(c);
            if(lastop != 'X')
                throw "Assigning `copayble` by move construction to `variant`, calls a constructor.";
            if(lastassign != 'c')
                throw "Assigning `copayble` by move construction to `variant`, does not call copy assignment operator.";
            if(destroy)
                throw "Assigning `copayble` by move construction to `variant`, calls destructor.";
        }

        {
            variant <copyable, movable> x = movable();
            reset();
            x = std :: move(m);
            if(lastop != 'X')
                throw "Assigning `movable` by move construction to `variant`, calls a constructor.";
            if(lastassign != 'm')
                throw "Assigning `movable` by move construction to `variant`, does not call move assignment operator.";
            if(destroy)
                throw "Assigning `movable` by move construction to `variant`, calls destructor.";
        }
    });

    $test("variant/assignment-copyvariant", []
    {
        variant <copyable, movable> c = copyable();
        variant <copyable, movable> m = movable();

        {
            variant <copyable, movable> x = copyable();
            reset();
            x = c;
            if(lastop != 'X')
                throw "Assigning a `variant` of type `copayble` by copy construction to `variant`, calls a constructor.";
            if(lastassign != 'c')
                throw "Assigning a `variant` of type `copayble` by copy construction to `variant`, does not call copy assignment operator.";
            if(destroy)
                throw "Assigning a `variant` of type `copayble` by copy construction to `variant`, calls destructor.";
        }

        {
            variant <copyable, movable> x = movable();
            reset();
            x = m;
            if(lastop != 'X')
                throw "Assigning a `variant` of type `movable` by copy construction to `variant`, calls a constructor.";
            if(lastassign != 'c')
                throw "Assigning a `variant` of type `movable` by copy construction to `variant`, dos not call copy assignment operator.";
            if(destroy)
                throw "Assigning a `variant` of type `movable` by copy construction to `variant`, calls destructor.";
        }
    });

    $test("variant/assignment-movevariant", []
    {
        variant <copyable, movable> c = copyable();
        variant <copyable, movable> m = movable();

        {
            variant <copyable, movable> x = copyable();
            reset();
            x = std:: move(c);
            if(lastop != 'X')
                throw "Assigning a `variant` of type `copayble` by move construction to `variant`, calls a constructor.";
            if(lastassign != 'c')
                throw "Assigning a `variant` of type `copayble` by move construction to `variant`, does not call copy assignment operator.";
            if(destroy)
                throw "Assigning a `variant` of type `copayble` by move construction to `variant`, calls destructor.";
        }

        {
            variant <copyable, movable> x = movable();
            reset();
            x = std :: move(m);
            if(lastop != 'X')
                throw "Assigning a `variant` of type `movable` by move construction to `variant`, calls a constructor.";
            if(lastassign != 'm')
                throw "Assigning a `variant` of type `movable` by move construction to `variant`, does not call move assignment operator.";
            if(destroy)
                throw "Assigning a `variant` of type `movable` by move construction to `variant`, calls destructor.";
        }
    });

    $test("variant/assignment-anonymous", []
    {
        {
            variant <copyable, movable> x = copyable();
            reset();
            x = copyable();
            if(lastop != 'I')
                throw "Assigning anonymous `copayble` by copy construction to `variant`, does not call default constructor.";
            if(lastassign != 'c')
                throw "Assigning anonymous `copayble` by copy construction to `variant`, does not call copy assignment operator.";
            if(!destroy)
                throw "Assigning anonymous `copayble` by copy construction to `variant`, does not call destructor.";
        }

        {
            variant <copyable, movable> x = movable();
            reset();
            x = movable();
            if(lastop != 'I')
                throw "Assigning anonymous `movable` by copy construction to `variant`, does not call default constructor.";
            if(lastassign != 'm')
                throw "Assigning anonymous `movable` by copy construction to `variant`, does not call move assignment operator.";
            if(!destroy)
                throw "Assigning anonymous `movable` by copy construction to `variant`, does not call destructor.";
        }

        {
            variant <copyable, movable> x = copyable();
            reset();
            x = std :: move(copyable());
            if(lastop != 'I')
                throw "Assigning anonymous `copayble` by move construction to `variant`, does not call default constructor.";
            if(lastassign != 'c')
                throw "Assigning anonymous `copayble` by move construction to `variant`, does not call copy assignment operator.";
            if(!destroy)
                throw "Assigning anonymous `copayble` by move construction to `variant`, does not call destructor.";
        }

        {
            variant <copyable, movable> x = movable();
            reset();
            x = std :: move(movable());
            if(lastop != 'I')
                throw "Assigning anonymous `movable` by move construction to `variant`, does not call default constructor.";
            if(lastassign != 'm')
                throw "Assigning anonymous `movable` by move construction to `variant`, does not call move assignment operator.";
            if(!destroy)
                throw "Assigning anonymous `movable` by move construction to `variant`, does not call destructor.";
        }
    });
};
