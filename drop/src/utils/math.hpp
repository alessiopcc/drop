#ifndef __src__utils__math__hpp
#define __src__utils__math__hpp

// Includes

#include "math.h"

namespace drop
{
    template <typename type, size_t size> constexpr type max(const type (& values)[size])
    {
        type max = values[0];

        for(size_t i = 0; i < size; i++)
            if(max < values[i])
                max = values[i];

        return max;
    }
};

#endif
