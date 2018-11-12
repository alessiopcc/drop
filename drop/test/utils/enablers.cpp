#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "utils/parameters.h"
#include "data/variant.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;
    char lastassign;

    // Methods

    void reset()
    {
        lastop = 'X';
        lastassign = 'x';
    }

    template <typename type> void testenabler()
    {
        reset();
        type x = type();
        if(lastop != 'I')
            throw "Default construction does not call default constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        reset();
        type y = x;
        if(lastop != 'C')
            throw "Copy construction does not call copy constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        reset();
        x = y;
        if(lastop != 'X')
            throw "Copy assignment calls constructor.";
        if(lastassign != 'c')
            throw "Copy assignment does not call copy assignment operator.";

        reset();
        type z = std :: move(y);
        if(lastop != 'M')
            throw "Move construction does not call move constructor.";
        if(lastassign != 'x')
            throw "Move construction calls assignment operator.";

        reset();
        x = std :: move(z);
        if(lastop != 'X')
            throw "Move assignment calls constructor.";
        if(lastassign != 'm')
            throw "Move assignment does not call move assignment operator.";
    }

    // Classes

    class myclass
    {
    public:

        myclass()
        {
            lastop = 'I';
        }

        myclass(const myclass &)
        {
            lastop = 'C';
        }

        myclass(myclass &&)
        {
            lastop = 'M';
        }

        myclass & operator = (const myclass &)
        {
            lastassign = 'c';
            return (*this);
        }

        myclass & operator = (myclass &&)
        {
            lastassign = 'm';
            return (*this);
        }
    };

    class copyconstructible : public myclass, public enablers :: copy_constructible <true>
    {
    };
    class notcopyconstructible : myclass, public enablers :: copy_constructible <false>
    {
    };

    class moveconstructible : public myclass, public enablers :: move_constructible <true>
    {
    };
    class notmoveconstructible : myclass, public enablers :: move_constructible <false>
    {
    };

    class copyassignable : public myclass, public enablers :: copy_assignable <true>
    {
    };
    class notcopyassignable : myclass, public enablers :: copy_assignable <false>
    {
    };

    class moveassignable : public myclass, public enablers :: move_assignable <true>
    {
    };
    class notmoveassignable : myclass, public enablers :: move_assignable <false>
    {
    };

    // Tests

    $test("utils/enablers-copy_constructible-true", []
    {
        testenabler <copyconstructible> ();
    });

    $test("utils/enablers-copy_constructible-false", []
    {
        reset();
        notcopyconstructible x = notcopyconstructible();
        if(lastop != 'I')
            throw "Default construction of does not call default constructor.";
        if(lastassign != 'x')
            throw "Copy construction of calls assignment operator.";

        if (std :: is_copy_constructible <notcopyconstructible> :: value)
            throw "`notcopyconstructible` is copy_constructible.";

        auto c = notcopyconstructible();

        reset();
        x = c;
        if(lastop != 'X')
            throw "Copy assignment of calls constructor.";
        if(lastassign != 'c')
            throw "Copy assignment of does not call copy assignment operator.";

        reset();
        auto m = std :: move(c);
        if(lastop != 'M')
            throw "Move construction of does not call move constructor.";
        if(lastassign != 'x')
            throw "Move construction of calls assignment operator.";

        reset();
        x = std :: move(m);
        if(lastop != 'X')
            throw "Move assignment of calls constructor.";
        if(lastassign != 'm')
            throw "Move assignment of does not call move assignment operator.";
    });

    $test("utils/enablers-move_constructible-true", []
    {
        testenabler <moveconstructible> ();
    });

    $test("utils/enablers-move_constructible-false", []
    {
        reset();
        notmoveconstructible x = notmoveconstructible();
        if(lastop != 'I')
            throw "Default construction does not call default constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        reset();
        auto c = x;
        if(lastop != 'C')
            throw "Copy construction does not call copy constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        reset();
        x = c;
        if(lastop != 'X')
            throw "Copy assignment calls constructor.";
        if(lastassign != 'c')
            throw "Copy assignment does not call copy assignment operator.";

        reset();
        auto m = std :: move(c);
        if(lastop != 'C')
            throw "Move construction does not call copy constructor.";
        if(lastassign != 'x')
            throw "Move construction calls assignment operator.";

        reset();
        x = std :: move(m);
        if(lastop != 'X')
            throw "Move assignment calls constructor.";
        if(lastassign != 'm')
            throw "Move assignment does not call move assignment operator.";
    });

    $test("utils/enablers-copy_assignable-true", []
    {
        testenabler <copyassignable> ();
    });

    $test("utils/enablers-copy_assignable-false", []
    {
        reset();
        notcopyassignable x = notcopyassignable();
        if(lastop != 'I')
            throw "Default construction does not call default constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        reset();
        auto c = x;
        if(lastop != 'C')
            throw "Copy construction does not call copy constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        if(std :: is_copy_assignable <notcopyassignable> :: value)
            throw "`notcopyassignable` is copy copy_assignable.";

        reset();
        auto m = std :: move(c);
        if(lastop != 'M')
            throw "Move construction does not call copy constructor.";
        if(lastassign != 'x')
            throw "Move construction calls assignment operator.";

        reset();
        x = std :: move(m);
        if(lastop != 'X')
            throw "Move assignment calls constructor.";
        if(lastassign != 'm')
            throw "Move assignment does not call move assignment operator.";
    });

    $test("utils/enablers-move_assignable-true", []
    {
        testenabler <moveassignable> ();
    });

    $test("utils/enablers-move_assignable-false", []
    {
        reset();
        notmoveassignable x = notmoveassignable();
        if(lastop != 'I')
            throw "Default construction does not call default constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        reset();
        auto c = x;
        if(lastop != 'C')
            throw "Copy construction does not call copy constructor.";
        if(lastassign != 'x')
            throw "Copy construction calls assignment operator.";

        reset();
        x = c;
        if(lastop != 'X')
            throw "Copy assignment calls constructor.";
        if(lastassign != 'c')
            throw "Copy assignment does not call copy assignment operator.";

        reset();
        auto m = std :: move(c);
        if(lastop != 'M')
            throw "Move construction does not call copy constructor.";
        if(lastassign != 'x')
            throw "Move construction calls assignment operator.";

        reset();
        x = std :: move(m);
        if(lastop != 'X')
            throw "Move assignment calls constructor.";
        if(lastassign != 'c')
        {
            std :: cout << lastassign << std :: endl;
            throw "Move assignment does not call move assignment operator.";
        }
    });
};
