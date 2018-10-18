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

    // Casting

    template <typename expr> constexpr expression :: evaluator <expr> :: operator bool () const
    {
        return sfinae :: valid <expr> :: value;
    }
};

#endif
