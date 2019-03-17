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

    template <typename type, typename... ptypes, std :: enable_if_t <(... && (std :: is_same <ptypes, promise <type>> :: value))> *> promise <type> any(const promise <type> & head, const ptypes & ... tail)
    {
        struct context
        {
            promise <type> aggregated;

            std :: array <std :: exception_ptr, 1 + sizeof...(ptypes)> exceptions;
            size_t rejections;

            guard <simple> guard;
        };

        std :: shared_ptr <context> context(new (struct context) {.rejections = 0});

        size_t index = 0;
        for(const auto & promise : {head, tail...})
        {
            [&]() -> drop :: promise <void>
            {
                struct // This is to override an overly-zealous capture optimizer.
                {
                    size_t index;
                    drop :: promise <type> promise;
                    std :: shared_ptr <struct context> context;
                } local{.index = index, .promise = promise, .context = context};

                try
                {
                    if constexpr(std :: is_same <type, void> :: value)
                    {
                        co_await local.promise;
                        local.context->aggregated.resolvesoft();
                    }
                    else
                    {
                        type value = co_await local.promise;
                        local.context->aggregated.resolvesoft(value);
                    }
                }
                catch(...)
                {
                    bool reject = local.context->guard([&]()
                    {
                        local.context->exceptions[local.index] = std :: current_exception();
                        local.context->rejections++;

                        return (local.context->rejections == (1 + sizeof...(ptypes)));
                    });

                    if(reject)
                        local.context->aggregated.reject(local.context->exceptions);
                }
            }();

            index++;
        }

        return context->aggregated;
    }
};

#endif
