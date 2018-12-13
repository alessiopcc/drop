// Includes

#include "guard.hpp"

namespace drop
{
    // guard <hard> :: shield

    // Constructors

    guard <hard> :: shield :: shield(std :: mutex & mutex) : _mutex(mutex)
    {
        this->_mutex.lock();
    }

    // Destructor

    guard <hard> :: shield :: ~shield()
    {
        this->_mutex.unlock();
    }
};
