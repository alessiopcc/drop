// Forward declarations

namespace drop
{
    // Tags

    class bad_access;
    class already_resolved;

    // Classes

    template <typename> class promise;
};

#if !defined(__forward__) && !defined(__src__async__promise__h)
#define __src__async__promise__h

// Libraries

#include <experimental/coroutine>
#include <memory>
#include <exception>
#include <vector>

// Includes

#include "drop/data/variant.hpp"
#include "drop/exception/exception.hpp"
#include "drop/thread/guard.hpp"

namespace drop
{
    template <typename type> class promise
    {
    public:

        // Constraints

        struct constraints
        {
            template <typename...> static constexpr bool resolve();
        };

    private:

        // Service typedefs

        typedef variant <std :: experimental :: coroutine_handle <>, std :: vector <std :: experimental :: coroutine_handle <>>> handles;

        // Service nested classes

        class arc;
        class null;

    public:

        // Coroutine interface

        class promise_base;
        class promise_type;

    private:

        // Members

        std :: shared_ptr <arc> _arc;

    public:

        // Constructors

        promise();

        // Awaitable interface

        bool await_ready() const;
        void await_suspend(std :: experimental :: coroutine_handle <>) const;
        auto await_resume() const;

        // Methods

        template <typename... vtype, std :: enable_if_t <constraints :: template resolve <vtype...> ()> * = nullptr> void resolve(const vtype & ...) const;

        void reject(const std :: exception_ptr &) const;
        template <typename etype> void reject(const etype &) const;

    private:

        // Private static methods

        static void flush(handles &);
    };

    template <typename type> class promise <type> :: promise_base
    {
    protected:

        // Members

        promise <type> _promise;

    public:

        // Object

        inline auto get_return_object();

        // Suspends

        inline auto initial_suspend();
        inline auto final_suspend();

        // Exceptions

        inline void unhandled_exception();
    };

    template <> class promise <void> :: promise_type : public promise <void> :: promise_base
    {
    public:

        // Returns

        inline void return_void();
    };

    template <typename type> class promise <type> :: promise_type : public promise <type> :: promise_base
    {
    public:

        // Returns

        inline void return_value(const type &);
    };

    template <typename type> class promise <type> :: arc
    {
        // Friends

        friend class promise <type>;

        // Typedefs

        typedef std :: conditional_t <std :: is_same <type, void> :: value, null, type> storetype;

        // Members

        variant <storetype, std :: exception_ptr> _status;
        handles _handles;
        bool _resumed;

        guard <simple> _guard;

    public:

        // Constructors

        arc();

        // Destructor

        ~arc();
    };

    template <typename type> class promise <type> :: null
    {
    };
};

#endif
