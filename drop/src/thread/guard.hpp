#if !defined(__src__thread__guard__hpp)
#define __src__thread__guard__hpp

// Includes

#include "guard.h"
#include "shield.hpp"

namespace drop
{
    // guard <recursive>

    // Operators

    template <typename type> template <typename lambda> auto guard <type> :: operator () (lambda && operation)
    {
        shield <type> shield(this->_mutex);
        return operation();
    }

    // guard <sequential>

    // Operators

    template <typename lambda> void guard <sequential> :: operator () (lambda && operation)
    {
        shield <recursive> shield(this->_mutex);

        if(this->_lock)
            this->_queue.push_back(operation);
        else
        {
            this->_lock = true;
            operation();
            this->_lock = false;

            if(!this->_queue.empty())
            {
                auto next = this->_queue.front();
                this->_queue.pop_front();
                (*this)(next);
            }
        }
    }
};

#endif
