#ifndef __src__utils__parameters__hpp
#define __src__utils__parameters__hpp

// Includes

#include "parameters.h"

namespace drop
{
    namespace
    {
        // Helper functions

        template <size_t times, typename type, typename lambda, typename... types> void repeatloop(type && original, lambda && callback, types && ... copies)
        {
            if constexpr (sizeof...(copies) == times)
                callback(std :: forward <types> (copies)...);
            else
                repeatloop <times> (std :: forward <type> (original), std :: forward <lambda> (callback), std :: forward <type> (original), std :: forward <types> (copies)...);
        }
    };

    namespace parameters
    {
        // Functions

        template <size_t times, typename type, typename lambda> void repeat(type && original, lambda && callback)
        {
            repeatloop <times> (std :: forward <type> (original), std :: forward <lambda> (callback));
        }    
    }
};

#endif
