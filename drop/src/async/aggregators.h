#if !defined(__forward__) && !defined(__src__async__aggregators__h)
#define __src__async__aggregators__h

// Includes

#include "promise.hpp"

namespace drop
{
    // Functions

    template <typename... ptypes, std :: enable_if_t <(sizeof...(ptypes) > 0) && (... && (std :: is_same <ptypes, promise <void>> :: value))> * = nullptr> promise <void> all(const ptypes & ...);
};

#endif
