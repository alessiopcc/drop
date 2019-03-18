#ifndef __src__data__syncset__syncset__path__hpp
#define __src__data__syncset__syncset__path__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: path :: path(const hash & path) : hash(path)
    {
    }

    template <typename type> syncset <type> :: path :: path(const type & element) : hash(element)
    {
    }

    // Operators

    template <typename type> bool syncset <type> :: path :: operator [] (const size_t & index) const
    {
        size_t byte = index / 8;
        size_t bit = index % 8;

        return (((const hash &) *this)[byte]) & (1 << (7 - bit));
    }
};

#endif
