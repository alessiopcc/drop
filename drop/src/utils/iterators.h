// Forward declarations

namespace drop
{
    class iterators;
};

#if !defined(__forward__) && !defined(__src__utils__iterators__h)
#define __src__utils__iterators__h

// Libraries

#include <tuple>

// Includes

#include "drop/concept/expression.hpp"
#include "drop/concept/stltraits.h"

namespace drop
{
    class iterators
    {
    public:

        // Constraints

        class constraints
        {
            // Constraint helpers

            template <bool, typename, size_t, typename> static constexpr bool eachloop();

        public:

            // Constraints

            template <typename, typename> static constexpr bool each();
        };

        // Static methods

        template <typename ttype, typename lambda, std :: enable_if_t <constraints :: each <ttype, lambda> ()> * = nullptr> static void each(ttype &&, lambda &&);

    private:

        // Private static methods

        template <size_t index, typename ttype, typename lambda> static void eachloop(ttype &&, lambda &&);
    };
};

#endif
