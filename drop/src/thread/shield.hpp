#if !defined(__src__thread__shield__hpp)
#define __src__thread__shield__hpp

// Includes

#include "shield.h"

namespace drop
{
    // Constructors

    template <typename type> shield <type> :: shield(std :: conditional_t <std :: is_same <type, simple> :: value, std :: mutex, std :: recursive_mutex> & mutex) : _mutex(mutex)
    {
        this->_mutex.lock();
    }

    // Destructor

    template <typename type> shield <type> :: ~shield()
    {
        this->_mutex.unlock();
    }
};

#endif
