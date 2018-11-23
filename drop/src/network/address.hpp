#ifndef __src__network__address__hpp
#define __src__network__address__hpp

// Includes

#include "address.h"

namespace drop
{
    // address

    // Methods

    template <typename... lambdas, std :: enable_if_t <variant <sockaddr_in, sockaddr_in6> :: constraints :: match <false, lambdas...> ()> *> void address :: match(lambdas && ... matchcases)
    {
        this->_sockaddr.match(matchcases...);
    }

    template <typename... lambdas, std :: enable_if_t <variant <sockaddr_in, sockaddr_in6> :: constraints :: match <true, lambdas...> ()> *> void address :: match(lambdas && ... matchcases) const
    {
        this->_sockaddr.match(matchcases...);
    }

    // ip

    // Methods

    template <typename... lambdas, std :: enable_if_t <variant <in_addr, in6_addr> :: constraints :: match <false, lambdas...> ()> *> void address :: ip :: match(lambdas && ... matchcases)
    {
        this->_addr.match(matchcases...);
    }

    template <typename... lambdas, std :: enable_if_t <variant <in_addr, in6_addr> :: constraints :: match <true, lambdas...> ()> *> void address :: ip :: match(lambdas && ... matchcases) const
    {
        this->_addr.match(matchcases...);
    }
};

#endif
