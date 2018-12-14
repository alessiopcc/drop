// Forward declarations

namespace drop
{
    class pool;
};

#if !defined(__forward__) && !defined(__src__network__pool__h)
#define __src__network__pool__h

// Libraries

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <thread>

// Includes

#include "tcp.hpp"
#include "streamer.hpp"
#include "queue.hpp"
#include "bytewise/bytewise.hpp"
#include "crypto/shorthash.hpp"
#include "chrono/time.hpp"
#include "data/variant.hpp"
#include "async/promise.hpp"
#include "thread/guard.hpp"

namespace drop
{
    class pool
    {
        // Service nested classes

        struct event;
        struct timeout;
        struct context;
        struct task;

        // Members

        queue _queue;

        struct
        {
            std :: unordered_map <int, task> write;
            std :: unordered_map <int, task> read;
        } _tasks;

        std :: vector <timeout> _timeouts;
        std :: vector <event> _pending;

        guard <soft> _guard;
    };

    struct pool :: event
    {
        queue :: type type;
        int descriptor;
    };

    struct pool :: timeout
    {
        // Members

        event event;
        uint64_t nonce;
        timestamp timeout;

        // Operators

        bool operator < (const struct timeout &) const;
    };

    struct pool :: context
    {
        variant <tcp :: socket> socket;
        variant <streamer <send>, streamer <receive>> streamer;
    };

    struct pool :: task
    {
        promise <void> promise;
        optional <context> context;
        uint64_t nonce;
    };
};

#endif
