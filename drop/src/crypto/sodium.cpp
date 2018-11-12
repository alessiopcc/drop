// Includes

#include "sodium.h"
#include "exception/exception.hpp"

namespace drop
{
    // Private constructors

    sodium :: sodium()
    {
        if(sodium_init() < 0)
            exception <sodium, init_failed> :: raise();
    }

    // Private static members

    sodium sodium :: initializer;
};
