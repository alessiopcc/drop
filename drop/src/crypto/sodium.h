namespace drop
{
    // Tags

    class init_failed;

    // Classes

    class sodium;
};

#if !defined(__forward__) && !defined(__src__crypto__sodium__h)
#define __src__crypto__sodium__h

// Libraries

#include <sodium.h>

namespace drop
{
    class sodium
    {
        // Private constructors

        sodium();

        // Private static members

        static sodium initializer;
    };
};

#endif
