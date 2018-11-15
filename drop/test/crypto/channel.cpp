#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/channel.hpp"

namespace
{
    // Using

    using namespace drop;

    template <typename type> void printtype(const type &)
    {
        std :: cout << __PRETTY_FUNCTION__ << std :: endl;
    }

    // Tests

    $test("channel/develop", []
    {
        channel alice;
        channel bob(alice.key(), alice.nonce());

        uint32_t alpha = 99;
        uint32_t beta = 14;
        uint32_t gamma = 6;
        std :: vector <uint32_t> delta = {1, 2};

        auto ciphertext = alice.encrypt(alpha, beta, gamma, delta);
        auto [otheralpha, otherbeta, othergamma, otherdelta] = bob.decrypt <uint32_t, uint32_t, uint32_t, std :: vector <uint32_t>> (ciphertext);

        std :: cout << otheralpha << std :: endl;
        std :: cout << otherbeta << std :: endl;
        std :: cout << othergamma << std :: endl;

        std :: cout << otherdelta[0] << std :: endl;
        std :: cout << otherdelta[1] << std :: endl;
    });
};
