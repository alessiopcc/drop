#ifndef __src__network__pool__hpp
#define __src__network__pool__hpp

// Includes

#include "pool.h"

namespace drop
{
    // pool

    // Constraints

    template <typename type> constexpr bool pool :: constraints :: socket()
    {
        return std :: is_same <type, tcp :: socket> :: value;
    }

    // Methods

    template <typename type, std :: enable_if_t <pool :: constraints :: socket <type> ()> *> promise <void> pool :: write(const type & socket, const interval & timeout)
    {
        task task;
        this->add(queue :: write, socket.template get <descriptor> (), task, timeout);
        return task.promise;
    }

    template <typename type, std :: enable_if_t <pool :: constraints :: socket <type> ()> *> promise <void> pool :: write(const type & socket, streamer <send> & streamer, const interval & timeout)
    {
        task task{.context = context{.socket = socket, .streamer = &streamer}};
        this->add(queue :: write, socket.template get <descriptor> (), task, timeout);
        return task.promise;
    }

    template <typename type, std :: enable_if_t <pool :: constraints :: socket <type> ()> *> promise <void> pool :: read(const type & socket, const interval & timeout)
    {
        task task;
        this->add(queue :: read, socket.template get <descriptor> (), task, timeout);
        return task.promise;
    }

    template <typename type, std :: enable_if_t <pool :: constraints :: socket <type> ()> *> promise <void> pool :: read(const type & socket, streamer <receive> & streamer, const interval & timeout)
    {
        task task{.context = context{.socket = socket, .streamer = &streamer}};
        this->add(queue :: read, socket.template get <descriptor> (), task, timeout);
        return task.promise;
    }
};

#endif
