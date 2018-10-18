#ifndef __src__concept__expression__hpp
#define __src__concept__expression__hpp

#include "expression.h"

namespace drop
{
    // expression

    // Static methods

    template <typename expr> constexpr expression :: evaluator <expr> expression :: evaluate(const expr &)
    {
        return evaluator <expr> ();
    }

    // evaluator

    // Methods

    template <typename expr> template <typename type> constexpr bool expression :: evaluator <expr> :: is() const
    {
        if constexpr (sfinae :: valid <expr> :: value)
            return std :: is_same <decltype(std :: declval <expr> ()(std :: declval <identity> ())), type> :: value;
        else
            return false;
    }

    template <typename expr> template <typename type> constexpr bool expression :: evaluator <expr> :: casts() const
    {
        if constexpr (sfinae :: valid <expr> :: value)
            return std :: is_convertible <decltype(std :: declval <expr> ()(std :: declval <identity> ())), type> :: value;
        else
            return false;
    }

    // Casting

    template <typename expr> constexpr expression :: evaluator <expr> :: operator bool () const
    {
        return sfinae :: valid <expr> :: value;
    }
};

#endif
