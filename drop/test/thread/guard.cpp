#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/thread/guard.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;

    // Tests

    $test("guard/recursive", []
    {
        guard <recursive> guard;

        {
            lastop = 'X';

            guard([&]()
            {
                if(lastop != 'X')
                    throw "`lastop` is not 'X' before 'A'.";
                lastop = 'A';

                guard([&](){
                    if(lastop != 'A')
                        throw "`lastop` is not 'A' before 'B'.";
                    lastop = 'B';

                    guard([&](){
                        if(lastop != 'B')
                            throw "`lastop` is not 'B' before 'C'.";
                        lastop = 'C';

                        guard([](){
                            if(lastop != 'C')
                                throw "`lastop` is not 'C' before 'D'.";
                            lastop = 'D';
                        });
                    });

                    if(lastop != 'D')
                        throw "`lastop` is not 'D' before 'E'.";
                    lastop = 'E';

                    guard([](){
                        if(lastop != 'E')
                            throw "`lastop` is not 'E' before 'F'.";
                        lastop = 'F';
                    });

                    if(lastop != 'F')
                        throw "`lastop` is not 'F' before 'G'.";
                    lastop = 'G';
                });

                if(lastop != 'G')
                    throw "`lastop` is not 'G' before 'H'.";
                lastop = 'H';

                guard([](){
                    if(lastop != 'H')
                        throw "`lastop` is not 'H' before 'I'.";
                    lastop = 'I';
                });

                if(lastop != 'I')
                    throw "`lastop` is not 'I' before 'J'.";
                lastop = 'J';
            });
        }

        {
            lastop = 'X';

            try
            {
                guard([&]()
                {
                    throw guard([]()
                    {
                        return 11;
                    });

                    return 33;
                });
            }
            catch(...)
            {
                lastop = 'T';
            }

            if(lastop != 'T')
                throw "`guard <recursive>` does not throw an exception when it should.";

            int x = guard([&]()
            {
                return guard([]()
                {
                    return 11;
                });
            });

            if(x != 11)
                throw "`guard <recursive>` does not return the correct value";
        }
    });

    $test("guard/sequential", []
    {
        guard <sequential> guard;

        lastop = 'X';

        guard([&]()
        {
            if(lastop != 'X')
                throw "`lastop` is not 'X' before 'A'.";
            lastop = 'A';

            guard([&](){
                if(lastop != 'C')
                    throw "`lastop` is not 'C' before 'D'.";
                lastop = 'D';

                guard([&](){
                    if(lastop != 'G')
                        throw "`lastop` is not 'G' before 'H'.";
                    lastop = 'H';

                    guard([](){
                        if(lastop != 'I')
                            throw "`lastop` is not 'I' before 'J'.";
                        lastop = 'J';
                    });
                });

                if(lastop != 'D')
                    throw "`lastop` is not 'D' before 'E'.";
                lastop = 'E';

                guard([](){
                    if(lastop != 'H')
                        throw "`lastop` is not 'H' before 'I'.";
                    lastop = 'I';
                });

                if(lastop != 'E')
                    throw "`lastop` is not 'E' before 'F'.";
                lastop = 'F';
            });

            if(lastop != 'A')
                throw "`lastop` is not 'A' before 'B'.";
            lastop = 'B';

            guard([](){
                if(lastop != 'F')
                    throw "`lastop` is not 'F' before 'G'.";
                lastop = 'G';
            });

            if(lastop != 'B')
                throw "`lastop` is not 'B' before 'C'.";
            lastop = 'C';
        });
    });
};
