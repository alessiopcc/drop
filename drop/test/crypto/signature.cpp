#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/signature.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class myclass
    {
    public:

        // Bytewise

        template <typename vtype> void accept(bytewise :: reader <vtype> & reader) const
        {
            for(uint32_t i = 0; i < 1024; i++)
                reader.visit(i);
        }
    };

    // Tests

    $test("signature/signer", []
    {
        {
            signer mysigner;
            signer myothersigner;

            if(mysigner.publickey() == myothersigner.publickey() || mysigner.secretkey() == myothersigner.secretkey())
                throw "Different `signer` are initialized with the same keys.";

            auto mysignature = mysigner.sign(3);
            auto myothersignature = myothersigner.sign(3);

            if(mysignature == myothersignature)
                throw "Signing the same value with `signer` with different keys returns the same output.";

            mysignature = mysigner.sign(5, 'r', 'a', 'i', 'n', myclass());
            myothersignature = myothersigner.sign(5, 'r', 'a', 'i', 'n', myclass());

            if(mysignature == myothersignature)
                throw "Calling `sign` with the same objects as input on two `signer` with different keys returns the same output.";
        }

        {
            signer mysigner;
            signer myothersigner= signer(mysigner.publickey(), mysigner.secretkey());

            if(mysigner.publickey() != myothersigner.publickey() || mysigner.secretkey() != myothersigner.secretkey())
                throw "`signer` constructurs does not initialized `signer` with the given keys.";

            auto mysignature = mysigner.sign(5, 'r', 'a', 'i', 'n');
            auto myothersignature = myothersigner.sign(5, 'r', 'a', 'i', 'n');
            if(mysigner.publickey() != myothersigner.publickey() || mysigner.secretkey() != myothersigner.secretkey())
                throw "Calling `sign` with the same inputs on two `signer` with identical keys returns different outputs.";

            mysignature = mysigner.sign(5, 'r', 'a', 'i', 'n', myclass());
            myothersignature = myothersigner.sign(5, 'r', 'a', 'i', 'n', myclass());
            if(mysignature != myothersignature)
                throw "Calling `sign` with the same objects as inputs on two `signer` with identical keys returns different outputs.";

            mysignature = mysigner.sign(std :: vector<int> {3, 2, 1, 0}, 5);
            myothersignature = myothersigner.sign(std :: vector<int> {3, 2, 1, 0}, 5);
            if(mysignature != myothersignature)
                throw "Signing the same vectors with two `signer` with identical keys returns different outputs.";
        }
    });

    $test("signature/verifier", []
    {
        signer mysigner;
        verifier myverifier(mysigner.publickey());

        if(myverifier.publickey() != mysigner.publickey())
            throw "`verifier` is not initialized with the correct key, or `publickey` getter returns the wrong key.";

        auto signature = mysigner.sign(511);
        try
        {
            myverifier.verify(signature, 511);
        }
        catch (exception <verification_failed> &)
        {
            throw "Verifing valid signature of single value throws an eception.";
        }

        signature = mysigner.sign(std :: string("Attack at dawn!"), 5, 'r', 'a', 'i', 'n', myclass());
        try
        {
            myverifier.verify(signature, std :: string("Attack at dawn!"), 5, 'r', 'a', 'i', 'n', myclass());
        }
        catch (exception <verification_failed> &)
        {
            throw "Verifing valid signature throws an eception.";
        }

        signature = mysigner.sign(511);
        bool thrown = false;
        try
        {
            myverifier.verify(signature, 111);
        }
        catch (exception <verification_failed> &)
        {
            thrown = true;
        }
        if(!thrown)
            throw "Verifing signature with wrong value does not throw an exception.";

        signature = mysigner.sign(std :: string("Attack at dawn!"), 5, 'r', 'a', 'i', 'n', myclass());
        thrown = false;
        try
        {
            myverifier.verify(signature, std :: string("Wrong message..."));
        }
        catch (exception <verification_failed> &)
        {
            thrown = true;
        }
        if(!thrown)
            throw "Verifing signature with wrong objects does not throw an exception.";


        signer dummysigner;
        verifier wrongverifier(dummysigner.publickey());
        signature = mysigner.sign(111);
        thrown = false;
        try
        {
            wrongverifier.verify(signature, 111);
        }
        catch (exception <verification_failed> &)
        {
            thrown = true;
        }
        if(!thrown)
            throw "Verifing signature of a single value with a wrong `verifier` does not throw an exception.";

        signature = mysigner.sign(std :: string("Attack at dawn!"), 5, 'r', 'a', 'i', 'n', myclass());
        thrown = false;
        try
        {
            wrongverifier.verify(signature, std :: string("Attack at dawn!"), 5, 'r', 'a', 'i', 'n', myclass());
        }
        catch (exception <verification_failed> &)
        {
            thrown = true;
        }
        if(!thrown)
            throw "Verifing signature of multiple values and objects with a wrong `verifier` does not throw an exception.";
    });
};
