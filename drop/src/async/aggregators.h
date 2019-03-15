#if !defined(__forward__) && !defined(__src__async__aggregators__h)
#define __src__async__aggregators__h

// Libraries

#include <memory>

// Includes

#include "promise.hpp"
#include "drop/thread/guard.hpp"

namespace drop
{
    // Functions

    template <typename... ptypes, std :: enable_if_t <(sizeof...(ptypes) > 0) && (... && (std :: is_same <ptypes, promise <void>> :: value))> * = nullptr> promise <void> all(const ptypes & ...);
    template <typename type, typename... ptypes, std :: enable_if_t <(... && (std :: is_same <ptypes, promise <type>> :: value))> * = nullptr> promise <type> any(const promise <type> &, const ptypes & ...);
};

#endif
