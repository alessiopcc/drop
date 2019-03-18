#ifndef __src__data__syncset__syncset__empty__hpp
#define __src__data__syncset__syncset__empty__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Static members

    template <typename type> hash syncset <type> :: empty :: _label;

    // Getters

    template <typename type> const hash & syncset <type> :: empty :: label() const
    {
        return _label;
    }

    // Methods

    template <typename type> typename syncset <type> :: single syncset <type> :: empty :: fill(const type & element)
    {
        return single(element);
    }

    template <typename type> template <typename lambda> void syncset <type> :: empty :: traverse(lambda && callback) const
    {
    }
};

#endif
