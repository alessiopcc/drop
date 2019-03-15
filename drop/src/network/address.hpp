#ifndef __src__network__address__hpp
#define __src__network__address__hpp

// Includes

#include "address.h"

namespace drop
{
    // address

    // Bytewise

    template <typename atype> void address :: accept(bytewise :: reader <atype> & reader) const
    {
        this->_sockaddr.match([&](const sockaddr_in & sockaddr)
        {
            reader.visit(uint8_t(0));
            reader.visit(reinterpret_cast <const std :: array <uint8_t, 4> &> (sockaddr.sin_addr.s_addr));
            reader.visit(reinterpret_cast <const std :: array <uint8_t, 2> &> (sockaddr.sin_port));
        },[&](const sockaddr_in6 & sockaddr)
        {
            reader.visit(uint8_t(1));
            reader.visit(reinterpret_cast <const std :: array <uint8_t, 16> &> (sockaddr.sin6_addr.s6_addr));
            reader.visit(reinterpret_cast <const std :: array <uint8_t, 2> &> (sockaddr.sin6_port));
        });
    }

    template <typename atype> void address :: accept(bytewise :: writer <atype> & writer)
    {
        uint8_t type;
        writer.visit(type);

        if(!type)
        {
            this->_sockaddr.emplace <sockaddr_in> ();
            writer.visit(reinterpret_cast <std :: array <uint8_t, 4> &> (this->_sockaddr.reinterpret <sockaddr_in> ().sin_addr.s_addr));
            writer.visit(reinterpret_cast <std :: array <uint8_t, 2> &> (this->_sockaddr.reinterpret <sockaddr_in> ().sin_port));
        }
        else
        {
            this->_sockaddr.emplace <sockaddr_in6> ();
            writer.visit(reinterpret_cast <std :: array <uint8_t, 16> &> (this->_sockaddr.reinterpret <sockaddr_in6> ().sin6_addr.s6_addr));
            writer.visit(reinterpret_cast <std :: array <uint8_t, 2> &> (this->_sockaddr.reinterpret <sockaddr_in6> ().sin6_port));
        }
    }

    // Getters

    template <typename type, std :: enable_if_t <std :: is_same <type, IPv4> :: value || std :: is_same <type, IPv6> :: value> *> bool address :: is() const
    {
        if constexpr (std :: is_same <type, IPv4> :: value)
            return this->_sockaddr.is <sockaddr_in> ();
        else
            return this->_sockaddr.is <sockaddr_in6> ();
    }

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
