#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/introspection/introspection.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tags

    class mytag;
    class myothertag;
    class mymixedtag;

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
        const char * q;
        const int w;

        // Tags

        $tag(mytag, i);
        $tag(mytag, j);

        $tag(myothertag, z);

        $tag(mymixedtag, i);
        $tag(mymixedtag, j);
        $tag(mymixedtag, q);
        $tag(mymixedtag, w);

    public:

        // Constructors

        myotherclass(const int & i, const double & j, const char & z, const int & w) : i(i), j(j), z(z), w(w)
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

    class refvisitor
    {
    public:

        template <typename type, std :: enable_if_t <!(std :: is_const <type> :: value)> * = nullptr> void operator () (type &)
        {
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

    $test("introspection/next", []
    {
        if(introspection :: next <myclass :: __tag__, mytag, -1> () != 2)
            throw "Tag `mytag` found by `next` more or less than 2 times in class `myclass`.";
    });

    $test("introspection/count", []
    {
        class empty {};

        if(introspection :: count <myclass, mytag> () != 2)
            throw "Tag `mytag` found by `count` more or less than 2 times in class `myclass`.";

        if(introspection :: count <myclass, myothertag> () != 0)
            throw "Tag `myothertag` found by `count` more than 0 times in class `myclass`.";

        if(introspection :: count <empty, mytag> () != 0)
            throw "Tag `mytag` found by `count` more than 0 times in class `empty`.";

        if(introspection :: count <int, mytag> () != 0)
            throw "Tag `mytag` found by `count` more than 0 times in `int`.";
    });

    $test("introspection/get", []
    {
        myotherclass myobject(33, 3.3, '3', 3);
        introspection :: get <mytag, 0> (myobject) = 44;

        if(myobject.get_i() != 44)
            throw "First element of `mytag` was not properly modified.";

        if(introspection :: get <myothertag, 0> (myobject) != '3')
            throw "`const` element of `myothertag` is not correctly retrieved.";

        if(!(std :: is_same <decltype(introspection :: get <myothertag, 0> (myobject)), const char &> :: value))
            throw "`get` does not return a `const` reference from `const` element of `myothertag`";

        if(introspection :: get <mytag, 1> (myotherclass(55, 5.5, '5', 5)) != 5.5)
            throw "Second element of `const mytag` is not correctly retrieved.";
    });

    $test("introspection/visit", []
    {
        myotherclass myobject(11, 1.1, '1', 1);
        introspection :: visit <mytag> (myobject, [](auto && x)
        {
            x += 2;
        });

        if(myobject.get_i() != 13 || myobject.get_j() != 3.1)
            throw "Elements of `mytag` were not properly modified.";
    });

    $test("introspection/visitor", []
    {
        auto intlambda = [](const int &)
        {
        };

        auto anylambda = [](auto &&)
        {
        };

        if(!(introspection :: constraints :: visitor <decltype(intlambda), myotherclass, mytag> ()))
            throw "Lambda function accepting `const int &` cannot be used to visit `mytag`.";

        if(introspection :: constraints :: visitor <decltype(intlambda), myotherclass, mymixedtag> ())
            throw "Lambda function accepting `const int &` can be used to visit `mymixedtag`.";

        if(introspection :: constraints :: visitor <refvisitor, myotherclass, mymixedtag> ())
            throw "Reference visitor can be used to visit `mymixedtag`, which includes constant members.";

        if(!(introspection :: constraints :: visitor <decltype(anylambda), myotherclass, mytag> ()))
            throw "Lambda function accepting `auto &&` cannot be used to visit `mytag`.";

        if(!(introspection :: constraints :: visitor <decltype(anylambda), myotherclass, mymixedtag> ()))
            throw "Lambda function accepting `auto &&` cannot be used to visit `mymixedtag`.";
    });
};
