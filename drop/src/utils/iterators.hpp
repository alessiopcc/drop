#ifndef __src__utils__iterators__hpp
#define __src__utils__iterators__hpp

// Includes

#include "iterators.h"

namespace drop
{
    // Constraints

    template <typename lambda, bool consttup, typename... types> constexpr bool iterators :: constraints :: each()
    {
        if constexpr (consttup)
            return (... && ($expression($type(lambda)($type(const types &)))));
        else
            return (... && ($expression($type(lambda)($type(types &)))));
    }

    // Static methods

    template <typename... types, typename lambda, std :: enable_if_t <iterators :: constraints :: each <lambda, false, types...> ()> *> void iterators :: each(std :: tuple <types...> & tuple, lambda && callback)
    {
        eachloop <0> (tuple, callback);
    }

    template <typename... types, typename lambda, std :: enable_if_t <iterators :: constraints :: each <lambda, true, types...> ()> *> void iterators :: each(const std :: tuple <types...> & tuple, lambda && callback)
    {
        eachloop <0> (tuple, callback);
    }

    template <size_t index, typename ttype, typename lambda> void iterators :: eachloop(ttype && tuple, lambda && callback)
    {
        if constexpr (index < std :: tuple_size <std :: decay_t <ttype>> :: value)
        {
            callback(std :: get <index> (tuple));
            eachloop <index + 1> (tuple, callback);
        }
    }
};

#endif
