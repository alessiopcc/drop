#if !defined(__src__thread__guard__hpp)
#define __src__thread__guard__hpp

// Includes

#include "guard.h"

namespace drop
{
    // guard <hard>

    // Methods

    template <typename lambda> auto guard <hard> :: operator () (lambda && operation)
    {
        shield shield(this->_mutex);
        return operation();
    }
};

#endif
