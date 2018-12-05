#ifndef __src__crypto__signature__hpp
#define __src__crypto__signature__hpp

// Includes

#include "signature.h"
#include "exception/exception.hpp"

namespace drop
{
    // signer

    // Methods

    template <size_t size> signature signer :: sign(const std :: array <uint8_t, size> & message) const
    {
        signature signature;
        crypto_sign_detached(signature.data(), nullptr, message.data(), size, this->_secretkey.data());
        return signature;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> *> signature signer :: sign(const types & ... message) const
    {
        return this->sign(bytewise :: serialize(message...));
    }

    // verifier

    // Methods

    template <size_t size> void verifier :: verify(const signature & signature, const std :: array <uint8_t, size> & message) const
    {
        if(crypto_sign_verify_detached(signature.data(), message.data(), size, this->_publickey.data()))
            exception <verification_failed> :: raise(this);
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> *> void verifier :: verify(const signature & signature, const types & ... message) const
    {
        this->verify(signature, bytewise :: serialize(message...));
    }
};

#endif
