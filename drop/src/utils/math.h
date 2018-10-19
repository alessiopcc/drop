#if !defined(__forward__) && !defined(__src__utils__math__h)
#define __src__utils__math__h

// Libraries

#include <stddef.h>

namespace drop
{
    template <typename type, size_t size> constexpr type max(const type (&)[size]);
};

#endif
