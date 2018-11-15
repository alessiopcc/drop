// Includes

#include "box.hpp"

namespace drop
{
    // Constructors

    box :: box()
    {
        crypto_box_keypair(this->_publickey.data(), this->_secretkey.data());
    }

    box :: box(const class publickey & publickey, const class secretkey & secretkey) : _publickey(publickey), _secretkey(secretkey)
    {
    }

    // Getters

    const class box :: publickey & box :: publickey() const
    {
        return this->_publickey;
    }

    const class box :: secretkey & box :: secretkey() const
    {
        return this->_secretkey;
    }

    // Methods

    std :: vector <uint8_t> box :: encrypt(const class publickey & to, const std :: vector <uint8_t> & plaintext)
    {
        std :: vector <uint8_t> ciphertext(plaintext.size() + crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES);

        randombytes_buf(ciphertext.data(), crypto_box_NONCEBYTES);

        if(crypto_box_easy(ciphertext.data() + crypto_box_NONCEBYTES, plaintext.data(), plaintext.size(), ciphertext.data(), to.data(), this->_secretkey.data()))
            exception <encryption_failed, malformed_key> :: raise(this);

        return ciphertext;
    }

    std :: vector <uint8_t> box :: decrypt(const class publickey & from, const std :: vector <uint8_t> & ciphertext)
    {
        if(ciphertext.size() < crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES)
            exception <decryption_failed, message_too_short> :: raise(this);

        std :: vector <uint8_t> plaintext(ciphertext.size() - (crypto_secretbox_NONCEBYTES + crypto_secretbox_MACBYTES));

        if(crypto_box_open_easy(plaintext.data(), ciphertext.data() + crypto_box_NONCEBYTES, ciphertext.size() - crypto_box_NONCEBYTES, ciphertext.data(), from.data(), this->_secretkey.data()))
            exception <encryption_failed> :: raise(this);

        return plaintext;
    }
};
