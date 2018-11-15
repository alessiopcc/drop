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

        std :: vector <uint32_t> original = {1, 2, 3, 4, 5};

        auto plaintext = bytewise :: serialize(original);
        auto ciphertext = alice.encrypt(plaintext);

        auto recovered = bob.decrypt(ciphertext);
        auto delivered = bytewise :: deserialize <std :: vector <uint32_t>> (recovered);

        std :: cout << alice.key() << std :: endl;
        std :: cout << plaintext << std :: endl;
        std :: cout << ciphertext << std :: endl;
        std :: cout << recovered << std :: endl;

        std :: cout << delivered[0] << std :: endl;
        std :: cout << delivered[1] << std :: endl;
        std :: cout << delivered[2] << std :: endl;
        std :: cout << delivered[3] << std :: endl;
        std :: cout << delivered[4] << std :: endl;
    });
};
