// Includes

#include "shield.h"

namespace drop
{
    // Constructors

    shield :: shield(std :: recursive_mutex & mutex) : _mutex(mutex)
    {
        this->_mutex.lock();
    }

    // Destructor

    shield :: ~shield()
    {
        this->_mutex.unlock();
    }
};
