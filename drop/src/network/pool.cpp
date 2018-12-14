// Includes

#include "pool.hpp"

namespace drop
{
    // timeout

    // Operators

    bool pool :: timeout :: operator < (const struct timeout & rho) const
    {
        return this->timeout < rho.timeout;
    }
};
