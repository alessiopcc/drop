// Includes

#include "signature.hpp"

namespace drop
{
    // signer

    // Constructors

    signer :: signer()
    {
        crypto_sign_keypair(this->_publickey.data(), this->_secretkey.data());
    }

    signer :: signer(const class publickey & publickey, const class secretkey & secretkey) : _publickey(publickey), _secretkey(secretkey)
    {
    }

    // Getters

    const class signer :: publickey & signer :: publickey() const
    {
        return this->_publickey;
    }

    const class signer :: secretkey & signer :: secretkey() const
    {
        return this->_secretkey;
    }

    // Methods

    signature signer :: sign(const std :: vector <uint8_t> & message) const
    {
        signature signature;

        if(crypto_sign_detached(signature.data(), nullptr, message.data(), message.size(), this->_secretkey.data()))
            exception <signature_failed, malformed_key> :: raise();

        return signature;
    }

    // verifier

    // Constructors

    verifier :: verifier(const class signer :: publickey & publickey) : _publickey(publickey)
    {
    }

    // Getters

    const class signer :: publickey & verifier :: publickey() const
    {
        return this->_publickey;
    }

    // Methods

    void verifier :: verify(const signature & signature, const std :: vector <uint8_t> & message) const
    {
        if(crypto_sign_verify_detached(signature.data(), message.data(), message.size(), this->_publickey.data()))
            exception <verification_failed> :: raise(this);
    }
};
