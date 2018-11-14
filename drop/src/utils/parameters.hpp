#ifndef __src__utils__parameters__hpp
#define __src__utils__parameters__hpp

// Includes

#include "parameters.h"

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
        // Functions

        template <size_t times, typename type, typename lambda> auto repeat(type && original, lambda && callback)
        {
            return repeatloop <times> (std :: forward <type> (original), std :: forward <lambda> (callback));
        }
    }
};

#endif
