#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/keyexchanger.h"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("keyexchanger/develop", []
    {
        keyexchanger alice;
        keyexchanger bob;

        std :: cout << "Alice pubkey: " << alice.publickey() << std :: endl;
        std :: cout << "Bob pubkey: " << bob.publickey() << std :: endl << std :: endl;

        auto alicekp = alice.exchange(bob.publickey());
        auto bobkp = bob.exchange(alice.publickey());

        std :: cout << "Alice RX: " << alicekp.receive << std :: endl;
        std :: cout << "Alice TX: " << alicekp.transmit << std :: endl << std :: endl;

        std :: cout << "Bob RX: " << bobkp.receive << std :: endl;
        std :: cout << "Bob TX: " << bobkp.transmit << std :: endl << std :: endl;
    });
};
