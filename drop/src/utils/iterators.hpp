#ifndef __src__utils__iterators__hpp
#define __src__utils__iterators__hpp

// Includes

#include "iterators.h"

namespace drop
{
    // Constraint helpers

    template <bool consttup, typename ttype, size_t index, typename lambda> constexpr bool iterators :: constraints :: eachloop()
    {
        if constexpr (index < stltraits :: tuple <ttype> :: size)
        {
            if constexpr (consttup)
                return $expression($type(lambda)($type(const typename stltraits :: tuple <ttype> :: template type <index> &))) && eachloop <consttup, ttype, index + 1, lambda> ();
            else
                return $expression($type(lambda)($type(typename stltraits :: tuple <ttype> :: template type <index> &))) && eachloop <consttup, ttype, index + 1, lambda> ();
        }
        else
            return true;
    };

    // Constraints

    template <typename ttype, typename lambda> constexpr bool iterators :: constraints :: each()
    {
        if constexpr (stltraits :: tuple <std :: decay_t <ttype>> :: value)
            return eachloop <std :: is_const <std :: remove_reference_t <ttype>> :: value, std :: decay_t <ttype>, 0, lambda> ();
        else
            return false;
    }

    // Static methods

    template <typename ttype, typename lambda, std :: enable_if_t <iterators :: constraints :: each <ttype, lambda> ()> *> void iterators :: each(ttype && tuple, lambda && callback)
    {
        eachloop <0> (tuple, callback);
    }

    // Private static methods

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
