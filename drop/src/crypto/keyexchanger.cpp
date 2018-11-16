// Includes

#include "keyexchanger.h"
#include "exception/exception.hpp"

namespace drop
{
    // Constructors

    keyexchanger :: keyexchanger()
    {
        crypto_kx_keypair(this->_publickey.data(), this->_secretkey.data());
    }

    keyexchanger :: keyexchanger(const class publickey & publickey, const class secretkey & secretkey) : _publickey(publickey), _secretkey(secretkey)
    {
    }

    // Getters

    const class keyexchanger :: publickey & keyexchanger :: publickey() const
    {
        return this->_publickey;
    }

    const class keyexchanger :: secretkey & keyexchanger :: secretkey() const
    {
        return this->_secretkey;
    }

    // Methods

    keyexchanger :: keypair keyexchanger :: exchange(const class publickey & remote) const
    {
        keypair keypair;
        bool error;

        if(this->_publickey < remote)
            error = crypto_kx_server_session_keys(keypair.receive.data(), keypair.transmit.data(), this->_publickey.data(), this->_secretkey.data(), remote.data());
        else
            error = crypto_kx_client_session_keys(keypair.receive.data(), keypair.transmit.data(), this->_publickey.data(), this->_secretkey.data(), remote.data());

        if(error)
            exception <keyexchange_failed, malformed_key> :: raise(this);

        return keypair;
    }
};
