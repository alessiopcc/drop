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
#include "concept/expression.h"
#include "concept/stltraits.h"

// Macros

#define $bytewise(member) $tag(:: drop :: bytewise, member)

namespace drop
{
    // Classes

    class bytewise
    {
    public:

        // Constraints

        class constraints
        {
            // Constraint helpers

            template <typename, typename, size_t> static constexpr bool readableloop();
            template <typename, typename, size_t> static constexpr bool writableloop();

            template <typename, size_t> static constexpr bool fixedloop();

        public:

            // Constraints

            template <typename, typename> static constexpr bool readable();
            template <typename, typename> static constexpr bool writable();

            template <typename> static constexpr bool fixed();
        };

        // Nested classes

        template <typename> class reader;
        template <typename> class writer;
    };

    template <typename> class bytewise :: reader
    {
    };

    template <typename> class bytewise :: writer
    {
    };
};

#endif
