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

#include "concept/expression.hpp"

namespace drop
{
    class iterators
    {
    public:

        // Constraints

        struct constraints
        {
            template <typename, bool, typename...> static constexpr bool each();
        };

        // Static methods

        template <typename... types, typename lambda, std :: enable_if_t <constraints :: each <lambda, false, types...> ()> * = nullptr> static void each(std :: tuple <types...> &, lambda &&);
        template <typename... types, typename lambda, std :: enable_if_t <constraints :: each <lambda, true, types...> ()> * = nullptr> static void each(const std :: tuple <types...> &, lambda &&);

    private:

        // Private static methods

        template <size_t index, typename ttype, typename lambda> static void eachloop(ttype &&, lambda &&);
    };
};

#endif
