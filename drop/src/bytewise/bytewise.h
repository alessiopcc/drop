// Forward declarations

namespace drop
{
    class bytewise;
};

#if !defined(__forward__) && !defined(__src__bytewise__bytewise__h)
#define __src__bytewise__bytewise__h

// Libraries

#include <array>
#include <vector>
#include <stdint.h>

// Includes

#include "bytewise/endianess.h"
#include "introspection/introspection.h"

// Macros

#define $bytewise(member) $tag(:: drop :: bytewise, member)

namespace drop
{
    // Classes

    class bytewise
    {    
    };
};

#endif
