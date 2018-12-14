// Includes

#include "pool.hpp"

namespace drop
{
    // pool

    // Private methods

    void pool :: add(const queue :: type & type, const int & descriptor, task & task, const interval & timeout)
    {
        event event{.type = type, .descriptor = descriptor};

        this->_guard([&]()
        {
            task.nonce = this->_nonce;
            (type == queue :: write ? this->_tasks.write : this->_tasks.read)[descriptor] = task;

            this->settimeout(event, timeout);
            this->_pending.push_back(event);

            this->_nonce++;
        });
    }

    void pool :: settimeout(const struct event & event, const interval & timeout)
    {
        if((const uint64_t &) timeout)
        {
            this->_timeouts.push_back(
            {
                .event = event,
                .nonce = this->_nonce,
                .timeout = now() + timeout
            });

            std :: push_heap(this->_timeouts.begin(), this->_timeouts.end());
        }
    }

    // timeout

    // Operators

    bool pool :: timeout :: operator < (const struct timeout & rho) const
    {
        return this->timeout < rho.timeout;
    }
};
