// Includes

#include "guard.hpp"

namespace drop
{
    // guard <soft> :: shield

    // Constructors

    guard <soft> :: shield :: shield(std :: recursive_mutex & mutex) : _mutex(mutex)
    {
        this->_mutex.lock();
    }

    // Destructor

    guard <soft> :: shield :: ~shield()
    {
        this->_mutex.unlock();
    }
};
