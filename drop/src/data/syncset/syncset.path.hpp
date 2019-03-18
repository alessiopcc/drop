#ifndef __src__data__syncset__syncset__path__hpp
#define __src__data__syncset__syncset__path__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: path :: path(const hash & hash) : _hash(hash)
    {
    }

    template <typename type> syncset <type> :: path :: path(const type & element) : _hash(element)
    {
    }

    template <typename type> syncset <type> :: path :: path(bytewise) : _hash(bytewise{})
    {
    }

    // Operators

    template <typename type> typename syncset <type> :: navigation syncset <type> :: path :: operator [] (const size_t & index) const
    {
        size_t byte = index / 8;
        size_t bit = index % 8;

        return (this->_hash[byte] & (1 << (7 - bit))) ? right : left;
    }
};

#endif
