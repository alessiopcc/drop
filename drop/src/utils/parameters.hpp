#ifndef __src__utils__parameters__hpp
#define __src__utils__parameters__hpp

// Includes

#include "parameters.h"
#include "concept/expression.h"

#include <type_traits>

namespace drop
{
    namespace
    {
        // Helper functions

        template <size_t times, typename type, typename lambda, typename... types> auto repeatloop(type && original, lambda && callback, types && ... copies)
        {
            if constexpr (sizeof...(copies) == times)
                return callback(std :: forward <types> (copies)...);
            else
                return repeatloop <times> (std :: forward <type> (original), std :: forward <lambda> (callback), std :: forward <type> (original), std :: forward <types> (copies)...);
        }
    };

    namespace parameters
    {
        // Constraints Helpers

        template <size_t times, typename type, typename lambda, typename... types> constexpr bool constraints :: repeatloop()
        {
            if constexpr (sizeof...(types) < times)
                return constraints :: repeatloop <times, type, lambda, types..., type> ();
            else
                return $expression($type(lambda)($type(types)...));
        }

        // Constraints

        template <size_t times, typename type, typename lambda> constexpr bool constraints :: repeat()
        {
            return constraints :: repeatloop <times, type, lambda> ();
        }

        // Functions

        template <size_t times, typename type, typename lambda, std :: enable_if_t <constraints :: repeat <times, type, lambda> ()> *> auto repeat(type && original, lambda && callback)
        {
            return repeatloop <times> (std :: forward <type> (original), std :: forward <lambda> (callback));
        }
    }
};

#endif
