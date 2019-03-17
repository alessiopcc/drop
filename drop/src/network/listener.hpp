#ifndef __src__network__listener__hpp
#define __src__network__listener__hpp

// Includes

#include "listener.h"

namespace drop
{
    // Constraints

    template <typename type, typename... atypes> constexpr bool listener :: constraints :: construct()
    {
        return parameters :: in <type, variant> :: value && std :: is_constructible <type, atypes...> :: value;
    }

    // Getters

    template <> inline auto listener :: get <timeouts :: accept> () const
    {
        interval timeout;

        this->_arc->match([&](auto & listener)
        {
            if constexpr (!(std :: is_const <std :: remove_reference_t <decltype(listener)>> :: value))
                timeout = listener.template get <timeouts :: accept> ();
        });

        return timeout;
    }

    // Setters

    template <> inline void listener :: set <timeouts :: accept> (const interval & timeout) const
    {
        this->_arc->match([&](auto & listener)
        {
            if constexpr (!(std :: is_const <std :: remove_reference_t <decltype(listener)>> :: value))
                listener.template set <timeouts :: accept> (timeout);
        });
    }

    // Static methods

    template <typename type, typename... atypes, std :: enable_if_t <listener :: constraints :: construct <type, atypes...> ()> *> listener listener :: construct(atypes && ... args)
    {
        std :: shared_ptr <variant> arc = std :: make_shared <variant> ();
        arc->emplace <type> (std :: forward <atypes> (args)...);

        return listener(arc);
    }
};

#endif
