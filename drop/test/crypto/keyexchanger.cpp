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

    $test("keyexchanger", []
    {
        keyexchanger alice;
        keyexchanger bob;
        keyexchanger charlie(alice.publickey(), alice.secretkey());

        if(alice.publickey() == bob.publickey() || alice.secretkey() == bob.secretkey())
            throw "`keyexchanger` generates same keys twice in a row.";
        if(alice.publickey() != charlie.publickey() || alice.secretkey() != charlie.secretkey())
            throw "`keyexchanger` initilaized with the wrong keys.";

        auto alicekp = alice.exchange(bob.publickey());
        auto bobkp = bob.exchange(alice.publickey());

        if(alicekp.receive != bobkp.transmit)
            throw "Alice received something different than what Bob transmitted.";
        if(alicekp.transmit != bobkp.receive)
            throw "Bob received something different than what Alice transmitted.";
    });
};
