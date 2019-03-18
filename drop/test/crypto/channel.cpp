#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/crypto/channel.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class myotherclass
    {
    public:

        // Bytewise

        template <typename vtype> void accept(bytewise :: reader <vtype> & reader) const
        {
            for(int32_t j = 0; j < 5; j++)
                reader.visit(j);
        }

        template <typename vtype> void accept(bytewise :: writer <vtype> & writer)
        {
            for(int32_t j = 0; j < 5; j++)
            {
                int32_t i;
                writer.visit(i);

                if(j != i)
                    throw "The values obtained by `myotherclass` from the `writer` are not consistent with what previously provided to the `reader`.";
            }
        }
    };

    class myclass
    {
    public:

        // Members

        int32_t i = 9;
        double j = 9.99;
        int8_t k = 'q';
        myotherclass m;

        std :: array <uint8_t, 16> q = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        std :: array <std :: vector <std :: array <int32_t, 4>>, 2> h =
        {
            std :: vector <std :: array <int32_t, 4>> {
                {1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12}
            },
            std :: vector <std :: array <int32_t, 4>> {
                {13, 14, 15, 16},
                {17, 18, 19, 20}
            }
        };

        std :: string s = "RAIN rules!";

        // Bytewise

        $bytewise(i);
        $bytewise(k);
        $bytewise(m);
        $bytewise(q);
        $bytewise(h);
        $bytewise(s);

        // Constructors

        myclass()
        {
        }

        myclass(bytewise) : i(0), k(0), q{}, h{}, s("")
        {
        }
    };

    // Tests

    $test("channel/key", []
    {
        auto mykey = channel :: key :: random();
        auto myotherkey = channel :: key :: random();

        if(mykey == myotherkey)
            throw "`channel :: key :: random` generated the same key twice in a row.";
    });

    $test("channel/nonce", []
    {
        auto mynonce = channel :: nonce :: random();

        auto myothernonce = channel :: nonce :: random();

        if(mynonce == myothernonce)
            throw "`channel :: nonce :: random` generated the same nonce twice in a row.";

        auto specular = mynonce.specular();
        specular.back() ^= 0x80;

        if(mynonce != specular)
            throw "`channel :: nonce :: specular` does not return the correct value.";

        myothernonce = mynonce;
        mynonce++;
        ++myothernonce;

        if(mynonce != myothernonce)
            throw "`operator ++ ()` and `operator ++ (int)` give different values when called on the same nonce.";
    });

    $test("channel/constructors", []
    {

        {
            if(std :: is_copy_constructible <channel> :: value)
                throw "`channel` is copy constructible.";
            if(std :: is_move_constructible <channel> :: value)
                throw "`channel` is move constructible.";
            if(std :: is_copy_assignable <channel> :: value)
                throw "`channel` is copy assignable.";
            if(std :: is_move_assignable <channel> :: value)
                throw "`channel` is move assignable.";
        }

        {
            channel mychannel;
            channel myotherchannel;

            if(mychannel.key() == myotherchannel.key())
                throw "Calling twice the default constructor creates two channel with the same key.";
            if(mychannel.nonce() == myotherchannel.nonce())
                throw "Calling twice the default constructor creates two channel with the same nonce.";
        }

        {
            channel mychannel;
            channel myotherchannel(mychannel.key());

            if(mychannel.key() != myotherchannel.key())
                throw "Calling `myotherchannel(mychannel.key())` does not create a channel with the given key.";
            if(mychannel.nonce() == myotherchannel.nonce())
                throw "Calling `myotherchannel(mychannel.key())` results in two channels with the same nonce";
        }

        {
            channel mychannel;
            channel myotherchannel(mychannel.key(), mychannel.nonce());

            if(mychannel.key() != myotherchannel.key() || mychannel.nonce() != myotherchannel.nonce())
                throw "Calling `myotherchannel(mychannel.key(), mychannel.nonce())` does not create a channel with the given values.";

            auto nonce = mychannel.nonce();
            nonce++;
            ++nonce;

            if(nonce == mychannel.nonce() || nonce == myotherchannel.nonce())
                throw "Nonce getter return the `nonce` by reference.";
        }
    });

    $test("channel/encrypt", []
    {
        {
            channel base;
            channel samekey = channel(base.key());
            channel copy = channel(base.key(), base.nonce());

            auto a = base.encrypt(1);
            auto b = samekey.encrypt(1);
            auto c = copy.encrypt(1);
            if(a == b)
                throw "Encrypting the same value with the same key but different nonce gives the same output.";
            if(a != c)
                throw "Encrypting the same value with the same key and nonce gives different output.";

            b = base.encrypt(1);
            c = copy.encrypt(2);
            if(a == b)
                throw "Encrypting the same value twice in a row gives the same ciphertext.";
            if (b == c)
                throw "Encrypting different values with the same key and nonce gives the same output.";
        }

        {
            channel base;
            channel samekey = channel(base.key());
            channel copy = channel(base.key(), base.nonce());

            auto a = base.encrypt(std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            auto b = samekey.encrypt(std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            auto c = copy.encrypt(std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            if(a == b)
                throw "Encrypting the same objects with the same key but different nonce gives the same output.";
            if(a != c)
                throw "Encrypting the same objects with the same key and nonce gives different output.";

            b = base.encrypt(std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            c = copy.encrypt(std::vector <char> {'R', 'a', 'i', 'n'}, myclass());
            if(a == b)
                throw "Encrypting the same objects twice in a row gives the same ciphertext.";
            if (b == c)
                throw "Encrypting different objects with the same key and nonce gives the same output.";
        }
    });

    $test("channel/decrypt", []
    {
        channel alice;
        channel bob(alice.key(), alice.nonce());

        {
            auto ciphertext = alice.encrypt(3);
            if(bob.decrypt <int> (ciphertext) != 3)
                throw "Decrypting does not output the correct value when using correct key and nonce.";
        }

        {
            auto a = std :: vector <uint32_t> {1, 2, 3, 4, 5, 6, 7};
            myclass b;
            auto ciphertext = alice.encrypt(a, b);
            auto [deca, decb] = bob.decrypt <std :: vector <uint32_t>, myclass> (ciphertext);
            if(deca != a || decb.i != b.i || decb.j != b.j || decb.k != b.k || decb.q != b.q || decb.h != b.h || decb.s != b.s)
                throw "Decrypting does not output the correct objects when using correct key and nonce.";
        }

        {
            auto ciphertext = alice.encrypt(3);
            bob.decrypt <int> (ciphertext);
            bool thrown = false;
            try
            {
                bob.decrypt <int> (ciphertext);
            }
            catch(exception <decryption_failed, mac_mismatch>)
            {
                thrown = true;
            }
            if(!thrown)
            throw "Decrypting with the wrong non-sinchronized nonce does not throw an exception.";

            auto wrongkey = channel(channel :: key :: random (), alice.nonce());
            thrown = false;
            try
            {
                wrongkey.decrypt <int> (ciphertext);
            }
            catch(exception <decryption_failed, mac_mismatch>)
            {
                thrown = true;
            }
            if(!thrown)
                throw "Decrypting with the wrong key does not throw an exception.";

            auto wrongnonce = channel(alice.key());
            thrown = false;
            try
            {
                wrongnonce.decrypt <int> (ciphertext);
            }
            catch(exception <decryption_failed, mac_mismatch>)
            {
                thrown = true;
            }
            if(!thrown)
                throw "Decrypting with the wrong nonce does not throw an exception.";

            auto tooshort = std :: vector <uint8_t> {0, 0, 0};
            thrown = false;
            try
            {
                bob.decrypt(tooshort);
            }
            catch(exception <decryption_failed, message_too_short>)
            {
                thrown = true;
            }
            if(!thrown)
                throw "Trying to decrypt too short message does not throw an exception.";
        }
    });
};
