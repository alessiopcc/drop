// Forward declarations

namespace drop
{
    // Tags

    class simple;
    class recursive;

    // Classes

    template <typename> class shield;
};

#if !defined(__forward__) && !defined(__src__thread__shield__h)
#define __src__thread__shield__h

// Libraries

#include <mutex>

namespace drop
{
    template <typename type> class shield
    {
        // Asserts

        static_assert(std :: is_same <type, simple> :: value || std :: is_same <type, recursive> :: value, "Valid tags for `shield` are: `simple`, `recursive`.");

        // Members

        std :: conditional_t <std :: is_same <type, simple> :: value, std :: mutex, std :: recursive_mutex> & _mutex;

    public:

        // Constructors

        shield(std :: conditional_t <std :: is_same <type, simple> :: value, std :: mutex, std :: recursive_mutex> &);

        // Destructor

        ~shield();
    };
};

#endif
