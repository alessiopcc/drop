#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/channel.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("channel/develop", []
    {
        channel alice;
        channel bob(alice.key(), alice.nonce());

        uint32_t original = 1234;

        auto plaintext = bytewise :: serialize(original);
        auto ciphertext = alice.encrypt(plaintext);

        auto recovered = bob.decrypt(ciphertext);
        auto delivered = bytewise :: deserialize <uint32_t> (recovered);

        std :: cout << alice.key() << std :: endl;
        std :: cout << plaintext << std :: endl;
        std :: cout << ciphertext << std :: endl;
        std :: cout << recovered << std :: endl;
        std :: cout << delivered << std :: endl;
    });
};
