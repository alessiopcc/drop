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
    class myothertag;

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
        const char z;

        // Tags

        $tag(mytag, i);
        $tag(mytag, j);

        $tag(myothertag, z);

    public:

        // Constructors

        myotherclass(const int & i, const double & j, const char & z) : i(i), j(j), z(z)
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
        myotherclass myobject(33, 3.3, '3');
        introspection :: get <mytag, 0> (myobject) = 44;

        if(myobject.get_i() != 44)
            throw "First element of `mytag` was not properly modified.";

        if(introspection :: get <myothertag, 0> (myobject) != '3')
            throw "`const` element of `myothertag` is not correctly retrieved.";

        if(!(std :: is_same <decltype(introspection :: get <myothertag, 0> (myobject)), const char &> :: value))
            throw "`get` does not return a `const` reference from `const` element of `myothertag`";

        if(introspection :: get <mytag, 1> (myotherclass(55, 5.5, '5')) != 5.5)
            throw "Second element of `const mytag` is not correctly retrieved.";
    });

    $test("introspection/visit", []
    {
        myotherclass myobject(11, 1.1, '1');
        introspection :: visit <mytag> (myobject, [](auto && x)
        {
            x += 2;
        });

        if(myobject.get_i() != 13 || myobject.get_j() != 3.1)
            throw "Elements of `mytag` were not properly modified.";
    });
};
