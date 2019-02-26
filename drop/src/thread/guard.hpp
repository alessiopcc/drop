#if !defined(__src__thread__guard__hpp)
#define __src__thread__guard__hpp

// Includes

#include "guard.h"
#include "shield.h"

namespace drop
{
    // guard <recursive>

    // Operators

    template <typename lambda> auto guard <recursive> :: operator () (lambda && operation)
    {
        shield shield(this->_mutex);
        return operation();
    }

    // guard <sequential>

    // Operators

    template <typename lambda> auto guard <sequential> :: operator () (lambda && operation)
    {
        shield shield(this->_mutex);

        if(this->_lock)
            this->_queue.push_back(operation);
        else
        {
            this->_lock = true;
            operation();
            this->_lock = false;

            if(!this->_queue.empty())
            {
                this->_queue.front()();
                this->_queue.pop_front();
            }
        }
    }
};

#endif
