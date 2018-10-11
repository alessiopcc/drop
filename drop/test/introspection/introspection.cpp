#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "introspection/introspection.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tags

    class mytag;

    // Classes

    class myclass
    {
    public:

        template <typename, size_t, std :: nullptr_t> class __tag__;

        template <std :: nullptr_t __tagdummy__> class __tag__ <mytag, 0, __tagdummy__>
        {
        public:

            typedef void exists;
        };

        template <std :: nullptr_t __tagdummy__> class __tag__ <mytag, 1, __tagdummy__>
        {
        public:

            typedef void exists;
        };
    };

    class myotherclass
    {
        // Members

        int i;
        double j;

        // Tags

        $tag(mytag, i);
        $tag(mytag, j);

    public:

        // Constructors

        myotherclass(const int & i, const double & j) : i(i), j(j)
        {
        }

        // Getters

        const int & get_i()
        {
            return this->i;
        }

        const double & get_j()
        {
            return this->j;
        }
    };

    // Tests

    $test("introspection/exists", []
    {
        if(!(introspection :: exists <myclass :: __tag__, mytag, 0, -1> ()))
            throw "Tag `mytag <0>` not found in class `myclass`.";

        if(!(introspection :: exists <myclass :: __tag__, mytag, 1, -1> ()))
            throw "Tag `mytag <1>` not found in class `myclass`.";

        if(introspection :: exists <myclass :: __tag__, mytag, 2, -1> ())
            throw "Tag `mytag <2>` found in class `myclass`.";
    });

    $test("introspection/count", []
    {
        if(introspection :: count <myclass :: __tag__, mytag, -1> () != 2)
            throw "Tag `mytag` found more or less than 2 times in class `myclass`.";
    });

    $test("introspection/get", []
    {
        myotherclass myobject(33, 3.3);
        introspection :: get <mytag, 0> (myobject) = 44;
        std :: cout << myobject.get_i() << std :: endl;
    });

    $test("introspection/visit", []
    {
        myotherclass myobject(11, 1.1);
        introspection :: visit <mytag> (myobject, [](auto && x)
        {
            std :: cout << x + 5 << std :: endl;
            x += 2;
        });

        std :: cout << myobject.get_i() << std :: endl;
        std :: cout << myobject.get_j() << std :: endl;
    });
};
