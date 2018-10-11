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

        // Tags

        $tag(mytag, i);

    public:

        // Constructors

        myotherclass(const int & i) : i(i)
        {
        }

        // Getters

        const int & value()
        {
            return this->i;
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

    $test("introspection/macro", []
    {
        myotherclass myobject(33);
        introspection :: get <mytag, 0> (myobject) = 44;
        std :: cout << myobject.value() << std :: endl;
    });
};
