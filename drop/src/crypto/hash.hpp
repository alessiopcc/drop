#ifndef __src__crypto__hash__hpp
#define __src__crypto__hash__hpp

// Includes

#include "hash.h"

namespace drop
{
    // hash

    // Constructors

    template <typename... types, std :: enable_if_t <(... && (bytewise :: constraints :: readable <types, hash :: state> ()))> *> hash :: hash(const types & ... items) : hash(state().update(items...).finalize())
    {
    }

    // Static methods

    template <typename... types, std :: enable_if_t <(... && (bytewise :: constraints :: readable <types, hash :: state> ()))> *> hash hash :: keyed(const key & key, const types & ... items)
    {
        return state(key).update(items...).finalize();
    }

    // state

    // Methods

    template <typename... types, std :: enable_if_t <(... && (bytewise :: constraints :: readable <types, hash :: state> ()))> *> hash :: state & hash :: state :: update(const types & ... items)
    {
        (bytewise :: read(*this, items), ...);
        return (*this);
    }
};

#endif
