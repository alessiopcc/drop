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
        // Constraints

        struct constraints
        {
            template <typename> static constexpr bool socket();
        };

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

        uint64_t _nonce;
        guard <soft> _guard;

        // Methods

        template <typename type, std :: enable_if_t <constraints :: socket <type> ()> * = nullptr> promise <void> write(const type &, const interval & = 0);
        template <typename type, std :: enable_if_t <constraints :: socket <type> ()> * = nullptr> promise <void> write(const type &, const streamer <send> &, const interval & = 0);

        template <typename type, std :: enable_if_t <constraints :: socket <type> ()> * = nullptr> promise <void> read(const type &, const interval & = 0);
        template <typename type, std :: enable_if_t <constraints :: socket <type> ()> * = nullptr> promise <void> read(const type &, const streamer <receive> &, const interval & = 0);

    private:

        // Private methods

        void add(const queue :: type &, const int &, task &, const interval &);

        void settimeout(const struct event &, const interval &);
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
