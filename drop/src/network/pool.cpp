// Includes

#include "pool.hpp"

namespace drop
{
    // pool

    // Static members

    class pool :: system pool :: system;

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

            this->_pending.push_back(event);

            this->_nonce++;
        });

        this->_queue.wake();
    }

    promise <void> pool :: pop(const queue :: type & type, const int & descriptor)
    {
        this->_queue.remove(type, descriptor);

        return this->_guard([&]()
        {
            auto & taskmap = (type == queue :: write ? this->_tasks.write : this->_tasks.read);

            promise <void> promise = taskmap[descriptor].promise;
            taskmap.erase(descriptor);

            return promise;
        });
    }

    void pool :: run()
    {
        while(true)
        {
            this->update();
            this->select();
            this->collect();

            if(!(this->_guard([&](){return this->_alive;})))
                break;
        }
    }

    void pool :: update()
    {
        this->_guard([&]()
        {
            for(const event & event : this->_pending)
                this->_queue.add(event.type, event.descriptor);

            this->_pending.clear();
        });
    }

    void pool :: select()
    {
        this->_queue.select([&](const queue :: event & event)
        {
            this->handle(event);
        }, this->tick());
    }

    interval pool :: tick()
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

    void pool :: handle(const queue :: event & event)
    {
        if(!(event.error()))
        {
            optional <context> context = this->_guard([&]()
            {
                return (event.type() == queue :: write ? this->_tasks.write : this->_tasks.read)[event.descriptor()].context;
            });

            if(context)
            {
                (*context).socket.match([&](const auto & socket)
                {
                    (*context).streamer.match([&](auto * streamer)
                    {
                        if(streamer->stream(socket))
                            this->pop(event.type(), event.descriptor()).resolvesoft();
                    });
                });
            }
            else
                this->pop(event.type(), event.descriptor()).resolvesoft();
        }
        else
        {
            if(event.type() == queue :: write)
                this->pop(event.type(), event.descriptor()).rejectsoft(exception <write_failed> :: make(this));
            else
                this->pop(event.type(), event.descriptor()).rejectsoft(exception <read_failed> :: make(this));
        }
    }

    void pool :: collect()
    {
        std :: vector <event> rejections;

        this->_guard([&]()
        {
            timestamp threshold = now();
            while(this->_timeouts.size() && this->_timeouts.front().timeout <= threshold)
            {
                timeout timeout = this->_timeouts.front();

                std :: pop_heap(this->_timeouts.begin(), this->_timeouts.end());
                this->_timeouts.pop_back();

                auto & taskmap = (timeout.event.type == queue :: write) ? this->_tasks.write : this->_tasks.read;
                if(taskmap.find(timeout.event.descriptor) != taskmap.end() && taskmap[timeout.event.descriptor].nonce == timeout.nonce)
                    rejections.push_back(timeout.event);
            }
        });

        for(const auto & event : rejections)
        {
            if(event.type == queue :: write)
                this->pop(event.type, event.descriptor).rejectsoft(exception <write_timeout> :: make(this));
            else
                this->pop(event.type, event.descriptor).rejectsoft(exception <read_timeout> :: make(this));
        }
    }

    // timeout

    // Operators

    bool pool :: timeout :: operator < (const struct timeout & rho) const
    {
        return this->timeout > rho.timeout;
    }

    // system

    // Private static members

    thread_local size_t pool :: system :: roundrobin = 0;

    // Private constructors

    pool :: system :: system() : _pools(new pool[std :: thread :: hardware_concurrency()]), _size(std :: thread :: hardware_concurrency())
    {
    }

    // Destructor

    pool :: system :: ~system()
    {
        delete [] this->_pools;
    }

    // Methods

    pool & pool :: system :: get()
    {
        return this->_pools[(roundrobin++) % this->_size];
    }
};
