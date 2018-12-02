#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "crypto/box.hpp"

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

    /*
        TODO. exception <encryption_failed, malformed_key> :: raise(this);
    */

    $test("box/constructors", []
    {
        {
            box mybox;
            box myotherbox;

            if(mybox.publickey() == myotherbox.publickey() || mybox.secretkey() == myotherbox.secretkey())
                throw "Two `box` created in a row have the same keys.";
        }

        {
            box mybox;
            box myotherbox(mybox.publickey(), mybox.secretkey());

            if(mybox.publickey() != myotherbox.publickey() || mybox.secretkey() != myotherbox.secretkey())
                throw "Constrcutor does not assign the correct values to the keys.";
        }
    });

    $test("channel/encrypt", []
    {
        {
            box alice;
            box bob;
            box charlie;
            box eve(alice.publickey(), alice.secretkey());

            auto a = alice.encrypt(bob.publickey(), 1);
            auto b = alice.encrypt(charlie.publickey(), 1);
            if(a == b)
                throw "Encrypting the same value with different public keys gives the same output.";

            a = bob.encrypt(alice.publickey(), 1);
            b = charlie.encrypt(alice.publickey(), 1);
            if(a == b)
                throw "Encrypting the same value with different secret keys gives the same output.";

            b = bob.encrypt(alice.publickey(), 1);
            auto c = bob.encrypt(eve.publickey(), 1);
            if(a != b || a != c)
                throw "Encrypting the same value with the same keys does not give the same output.";

            b = bob.encrypt(alice.publickey(), 2);
            c = bob.encrypt(eve.publickey(), 2);
            if(a == b || a == c)
                throw "Encrypting different values gives the same output.";
        }

        {
            box alice;
            box bob;
            box charlie;
            box eve(alice.publickey(), alice.secretkey());

            auto a = alice.encrypt(bob.publickey(), std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            auto b = alice.encrypt(charlie.publickey(), std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            if(a == b)
                throw "Encrypting the same objects with different public keys gives the same output.";

            b = bob.encrypt(alice.publickey(), std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            b = charlie.encrypt(alice.publickey(), std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            if(a == b)
                throw "Encrypting the same objects with different secret keys gives the same output.";

            b = bob.encrypt(alice.publickey(), std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            auto c = bob.encrypt(eve.publickey(), std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            if(a != b || a != c)
                throw "Encrypting the same objects with the same keys does not give the same output.";

            b = bob.encrypt(alice.publickey(), std::vector <char> {'R', 'a', 'i', 'n'}, myclass());
            c = bob.encrypt(eve.publickey(), std::vector <char> {'R', 'a', 'i', 'n'}, myclass());
            if(a == b || a == c)
                throw "Encrypting different objects gives the same output.";
        }
    });

    $test("channel/decrypt", []
    {
        box alice;
        box bob;
        box charlie;
        bool thrown = false;

        {
            auto ciphertext = alice.encrypt(bob.publickey(), 3);
            if(bob.decrypt <int> (alice.publickey(), ciphertext))
                throw "Decrypting with the correct key does not return the corret value.";
        }

        {
            auto a = std :: vector <uint32_t> {1, 2, 3, 4, 5, 6, 7};
            myclass b;
            auto ciphertext = alice.encrypt(bob.publickey(), a, b);
            auto [deca, decb] = bob.decrypt <std :: vector <uint32_t>, myclass> (alice.publickey(), ciphertext);
            if(deca != a || decb.i != b.i || decb.j != b.j || decb.k != b.k || decb.q != b.q || decb.h != b.h || decb.s != b.s)
                throw "Decrypting withe the correct key does not return the correct objects.";
        }

        {
            auto ciphertext = alice.encrypt(bob.publickey(), 5);
            thrown = false;
            try
            {
                bob.decrypt <int> (charlie.publickey(), ciphertext);
            }
            catch(exception <decryption_failed>)
            {
                thrown = true;
            }
            if(!thrown)
                throw "Decrypting value with the wrong public key does not throw an exception.";

            thrown = false;
            try
            {
                charlie.decrypt <int> (alice.publickey(), ciphertext);
            }
            catch(exception <decryption_failed>)
            {
                thrown = true;
            }
            if(!thrown)
                throw "Decrypting value with the wrong secret key does not throw an exception.";
        }

        {
            auto ciphertext = alice.encrypt(bob.publickey(), std::vector <char> {'r', 'a', 'i', 'n'}, myclass());
            thrown = false;
            try
            {
                bob.decrypt <std::vector <char>, myclass> (charlie.publickey(), ciphertext);
            }
            catch(exception <decryption_failed>)
            {
                thrown = true;
            }
            if(!thrown)
                throw "Decrypting objects with the wrong public key does not throw an exception.";

            thrown = false;
            try
            {
                charlie.decrypt <std::vector <char>, myclass> (alice.publickey(), ciphertext);
            }
            catch(exception <decryption_failed>)
            {
                thrown = true;
            }
            if(!thrown)
                throw "Decrypting objects with the wrong secret key does not throw an exception.";
        }

        {
            auto tooshort = std :: vector <uint8_t> {0, 0, 0};
            thrown = false;
            try
            {
                bob.decrypt(alice.publickey(), tooshort);
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
