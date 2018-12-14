// Includes

#include "pool.hpp"

namespace drop
{
    // pool

    // Constructors

    pool :: pool() : _alive(true), _thread(&pool :: run, this)
    {
    }

    // Destructor

    pool :: ~pool()
    {
        this->_guard([&]()
        {
            this->_alive = false;
        });

        this->_queue.wake();
        this->_thread.join();
    }

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

        this->_queue.wake();
    }

    void pool :: run()
    {
        while(true)
        {
            std :: cout << "Hello World!" << std :: endl;

            this->_queue.select([](const queue :: event & event)
            {
                // TODO: Process each of these!
            }, this->nexttick());

            this->_guard([&]() // Adds the pending
            {
                for(const event & event : this->_pending)
                    this->_queue.add(event.type, event.descriptor);

                this->_pending.clear();
            });

            // TODO: Flush the timeouts

            if(!(this->_guard([&](){return this->_alive;})))
                break;
        }

        std :: cout << "Good night." << std :: endl;
    }

    interval pool :: nexttick()
    {
        if(this->_timeouts.size())
        {
            timestamp start = now();

            if(this->_timeouts.front().timeout > start)
                return std :: max(this->_timeouts.front().timeout - start, settings :: step);
            else
                return settings :: step;
        }

        return 0;
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
