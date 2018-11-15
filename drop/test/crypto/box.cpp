#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/box.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("box/develop", []
    {
        box alice;
        box bob;

        auto ciphertext = alice.encrypt(bob.publickey(), std :: array <uint32_t, 3> {33, 22, 11});

        std :: cout << bob.decrypt <std :: array <uint32_t, 3>> (alice.publickey(), ciphertext)[0] << std :: endl;
        std :: cout << bob.decrypt <std :: array <uint32_t, 3>> (alice.publickey(), ciphertext)[1] << std :: endl;
        std :: cout << bob.decrypt <std :: array <uint32_t, 3>> (alice.publickey(), ciphertext)[2] << std :: endl;
    });
};
