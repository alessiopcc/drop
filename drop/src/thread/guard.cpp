// Includes

#include "guard.hpp"

namespace drop
{
    // guard <recursive> :: shield

    // Constructors

    guard <recursive> :: shield :: shield(std :: recursive_mutex & mutex) : _mutex(mutex)
    {
        this->_mutex.lock();
    }

    // Destructor

    guard <recursive> :: shield :: ~shield()
    {
        this->_mutex.unlock();
    }
};
