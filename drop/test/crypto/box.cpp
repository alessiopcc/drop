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

        auto ciphertext = alice.encrypt(bob.publickey(), bytewise :: serialize(std :: vector <uint32_t> {33, 22, 11}));
        
        std :: cout << bytewise :: deserialize <std :: vector <uint32_t>> (bob.decrypt(alice.publickey(), ciphertext))[0] << std :: endl;
        std :: cout << bytewise :: deserialize <std :: vector <uint32_t>> (bob.decrypt(alice.publickey(), ciphertext))[1] << std :: endl;
        std :: cout << bytewise :: deserialize <std :: vector <uint32_t>> (bob.decrypt(alice.publickey(), ciphertext))[2] << std :: endl;
    });
};
