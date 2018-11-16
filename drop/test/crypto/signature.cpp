#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/signature.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("signature/develop", []
    {
        signer signer;
        verifier verifier(signer.publickey());

        signature signature = signer.sign(std :: string("Attack at dawn!"), 42);
        verifier.verify(signature, std :: string("Attack at dawn!"), 42);
    });
};
