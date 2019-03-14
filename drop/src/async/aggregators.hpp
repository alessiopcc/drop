#ifndef __src__async__aggregators__hpp
#define __src__async__aggregators__hpp

// Includes

#include "aggregators.h"

namespace drop
{
    // Functions

    template <typename... ptypes, std :: enable_if_t <(sizeof...(ptypes) > 0) && (... && (std :: is_same <ptypes, promise <void>> :: value))> *> promise <void> all(const ptypes & ... promises)
    {
        for(const auto & promise : {promises...})
            co_await promise;
    }
};

#endif
