#if !defined(__src__thread__guard__hpp)
#define __src__thread__guard__hpp

// Includes

#include "guard.h"

namespace drop
{
    // guard <soft>

    // Methods

    template <typename lambda> auto guard <soft> :: operator () (lambda && operation)
    {
        shield shield(this->_mutex);
        return operation();
    }
};

#endif
