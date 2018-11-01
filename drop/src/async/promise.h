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
#include <mutex>

// Includes

#include "data/variant.hpp"
#include "exception/exception.hpp"

namespace drop
{
    template <typename type> class promise
    {
        // Service nested classes

        class arc;
        class null;

    public:

        // Coroutine interface

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

        template <typename... dummy, std :: enable_if_t <sizeof...(dummy) == 0 && std :: is_same <type, void> :: value> * = nullptr> void resolve(dummy...) const;
        template <typename vtype, std :: enable_if_t <std :: is_same <vtype, type> :: value && !(std :: is_same <type, void> :: value)> * = nullptr> void resolve(const vtype &) const;
    };

    template <typename type> class promise <type> :: promise_type
    {
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

        // Returns

        template <typename... dummy, std :: enable_if_t <sizeof...(dummy) == 0 && std :: is_same <type, void> :: value> * = nullptr> inline void return_void(dummy...);
        template <typename vtype, std :: enable_if_t <std :: is_same <vtype, type> :: value && !(std :: is_same <type, void> :: value)> * = nullptr> inline void return_value(const vtype &);
    };

    template <typename type> class promise <type> :: arc
    {
        // Friends

        friend class promise <type>;

        // Typedefs

        typedef std :: conditional_t <std :: is_same <type, void> :: value, null, type> storetype;

        // Members

        variant <storetype, std :: exception_ptr> _status;
        variant <std :: experimental :: coroutine_handle <>, std :: vector <std :: experimental :: coroutine_handle <>>> _handles;

        std :: recursive_mutex _mutex;
    };

    template <typename type> class promise <type> :: null
    {
    };
};

#endif
