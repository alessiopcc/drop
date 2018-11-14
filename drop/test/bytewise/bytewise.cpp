#include "framework/test.hpp"

// Libraries

#include <iostream>
#include <iomanip>
#include <type_traits>

// Includes

#include "bytewise/bytewise.hpp"

namespace
{
    // Using

    using namespace drop;

    // Variables

    char lastop;

    // Classes

    class myotherclass
    {
    public:

        // Bytewise interface

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

        // Bytewise

        $bytewise(i);
        $bytewise(k);
        $bytewise(m);
        $bytewise(q);
        $bytewise(h);

        // Constructors

        myclass()
        {
            lastop = 'C';
        }

        myclass(bytewise) : i(0), k(0), q{}, h{}
        {
            lastop = 'B';
        }
    };

    class myotherfixedclass
    {
    public:

        // Members

        std :: array <int32_t, 3> a = {11, 22, 33};

        // Bytewise

        $bytewise(a);
    };

    class myfixedclass
    {
    public:

        // Members

        int32_t i = 4;
        int8_t k = '4';
        std :: array <myotherfixedclass, 4> q;

        // Bytewise

        $bytewise(i);
        $bytewise(k);
        $bytewise(q);

        // Constructors

        myfixedclass()
        {
            lastop = 'C';
        }

        myfixedclass(bytewise) : i(0), k(0), q{myotherfixedclass{.a = {}}, myotherfixedclass{.a = {}}, myotherfixedclass{.a = {}}, myotherfixedclass{.a = {}}}
        {
            lastop = 'B';
        }
    };

    class empty
    {
    };

    class nearlyempty
    {
        // Members

        empty e;

        // Bytewise

        $bytewise(e);
    };

    class myreader
    {
    public:

        uint8_t * data;
        size_t size;

        myreader(size_t alloc) : data(new uint8_t [alloc]), size(0)
        {
        }

        bool update(const uint8_t * chunk, size_t size)
        {
            memcpy(this->data + this->size, chunk, size);
            this->size += size;

            return false;
        }
    };

    class mywriter
    {
    public:

        uint8_t * data;
        size_t cursor;

        mywriter(uint8_t * data) : data(data), cursor(0)
        {
        }

        uint8_t * pop(size_t size)
        {
            size_t cursor = this->cursor;
            this->cursor += size;

            return (this->data + cursor);
        }
    };

    // Tests

    $test("bytewise/constraints", []
    {
        if(!(bytewise :: constraints :: readable <myotherclass, myreader> ()))
            throw "`myotherclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myotherclass, mywriter> ()))
            throw "`myotherclass` is not `mywriter` writable.";

        if(!(bytewise :: constraints :: readable <myclass, myreader> ()))
            throw "`myclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myclass, mywriter> ()))
            throw "`myclass` is not `mywriter` writable.";

        if(!(bytewise :: constraints :: readable <myotherfixedclass, myreader> ()))
            throw "`myotherfixedclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myotherfixedclass, mywriter> ()))
            throw "`myotherfixedclass` is not `mywriter` writable.";

        if(!(bytewise :: constraints :: readable <myfixedclass, myreader> ()))
            throw "`myfixedclass` is not `myreader` readable.";

        if(!(bytewise :: constraints :: writable <myfixedclass, mywriter> ()))
            throw "`myfixedclass` is not `mywriter` writable.";

        if(bytewise :: constraints :: fixed <myotherclass> ())
            throw "`myotherclass` is fixed.";

        if(bytewise :: constraints :: fixed <myclass> ())
            throw "`myotherclass` is fixed.";

        if(!(bytewise :: constraints :: fixed <myotherfixedclass> ()))
            throw "`myotherfixedclass` not is fixed.";

        if(!(bytewise :: constraints :: fixed <myfixedclass> ()))
            throw "`myfixedclass` is not fixed.";

        if(!(bytewise :: constraints :: fixed <int> ()))
            throw "`int` is not fixed.";

        if(bytewise :: constraints :: fixed <empty> ())
            throw "`empty` is fixed.";

        if(bytewise :: constraints :: readable <empty, myreader> ())
            throw "`empty` is `myreader` readable";

        if(bytewise :: constraints :: writable <empty, mywriter> ())
            throw "`empty` is `mywriter` writable";

        if(bytewise :: constraints :: fixed <nearlyempty> ())
            throw "`nearlyempty` is fixed.";

        if(bytewise :: constraints :: readable <nearlyempty, myreader> ())
            throw "`nearlyempty` is `myreader` readable";

        if(bytewise :: constraints :: writable <nearlyempty, mywriter> ())
            throw "`nearlyempty` is `mywriter` writable";

        if(!(bytewise :: constraints :: reader <myreader> ()))
            throw "`myreader` is not recognized as a reader.";

        if(bytewise :: constraints :: writer <myreader> ())
            throw "`myreader` is recognized as a writer.";

        if(!(bytewise :: constraints :: writer <mywriter> ()))
            throw "`mywriter` is not recognized as a writer.";

        if(bytewise :: constraints :: reader <mywriter> ())
            throw "`mywriter` is recognized as a reader.";
    });

    $test("bytewise/traits", []
    {
        if(bytewise :: traits :: size <myotherclass> () != 0)
            throw "`myotherclass` has non-zero `bytewise` size.";

        if(bytewise :: traits :: size <myclass> () != 0)
            throw "`myclass` has non-zero `bytewise` size.";

        if(bytewise :: traits :: size <uint32_t> () != 4)
            throw "The `bytewise` size of `uint32_t` is not 4.'";

        if(bytewise :: traits :: size <std :: array <std :: array <uint64_t, 4>, 13>> () != 416)
            throw "The `bytewise` size of `std :: array <std :: array <uint64_t, 4>, 13>` is not 416.";

        if(bytewise :: traits :: size <myotherfixedclass> () != 12)
            throw "The `bytewise` size of `myotherfixedclass` is not 396.";

        if(bytewise :: traits :: size <myfixedclass> () != 53)
            throw "The `bytewise` size of `myclass` is not 1589.";
    });

    $test("bytewise/read-write", []
    {
        myclass item;
        myreader reader(1048576);

        bytewise :: read(reader, item);

        std :: vector <uint8_t> reference =
        {
            9, 0, 0, 0,
            113,
                0, 0, 0, 0,
                1, 0, 0, 0,
                2, 0, 0, 0,
                3, 0, 0, 0,
                4, 0, 0, 0,
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
            3,
                1, 0, 0, 0,
                2, 0, 0, 0,
                3, 0, 0, 0,
                4, 0, 0, 0,
                5, 0, 0, 0,
                6, 0, 0, 0,
                7, 0, 0, 0,
                8, 0, 0, 0,
                9, 0, 0, 0,
                10, 0, 0, 0,
                11, 0, 0, 0,
                12, 0, 0, 0,
            2,
                13, 0, 0, 0,
                14, 0, 0, 0,
                15, 0, 0, 0,
                16, 0, 0, 0,
                17, 0, 0, 0,
                18, 0, 0, 0,
                19, 0, 0, 0,
                20, 0, 0, 0
        };

        if((reader.size != reference.size()) || memcmp(reader.data, reference.data(), reference.size()))
            throw "`read` method does not provide the correct sequence of bytes when serializing `myclass`.";

        myclass otheritem(bytewise{});
        mywriter writer(reader.data);

        bytewise :: write(writer, otheritem);

        if(writer.cursor != reader.size)
            throw "`write` method does not read the same number of bytes as produced by `read`.";

        if(
            otheritem.i != 9 ||
            otheritem.k != 'q' ||
            otheritem.q != std :: array <uint8_t, 16> {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0} ||
            otheritem.h != std :: array <std :: vector <std :: array <int32_t, 4>>, 2>
            {
                std :: vector <std :: array <int32_t, 4>>
                {
                    {1, 2, 3, 4},
                    {5, 6, 7, 8},
                    {9, 10, 11, 12}
                },
                std :: vector <std :: array <int32_t, 4>>
                {
                    {13, 14, 15, 16},
                    {17, 18, 19, 20}
                }
            }
        )
            throw "`write` method does not produce an object consistent with what provided to `read`.";
    });

    $test("bytewise/serialize-deserialize-fixed", []
    {
        uint32_t alpha = 99;
        myfixedclass beta;
        uint8_t gamma = 'x';

        auto data = bytewise :: serialize(alpha, beta, gamma);

        if(!(std :: is_same <decltype(data), std :: array <uint8_t, 58>> :: value))
            throw "`serialize` on `uint32_t, myfixedclass, uint8_t` does not return a properly sized `std :: array`.";

        std :: array <uint8_t, 58> reference =
        {
            99, 0, 0, 0,
            4, 0, 0, 0,
            52,
            11, 0, 0, 0, 22, 0, 0, 0, 33, 0, 0, 0,
            11, 0, 0, 0, 22, 0, 0, 0, 33, 0, 0, 0,
            11, 0, 0, 0, 22, 0, 0, 0, 33, 0, 0, 0,
            11, 0, 0, 0, 22, 0, 0, 0, 33, 0, 0, 0,
            120
        };

        if(data != reference)
            throw "`serialize` does not produce the correct sequence of bytes when serializing `uint32_t, myfixedclass, uint8_t`";

        lastop = 'X';
        auto [otheralpha, otherbeta, othergamma] = bytewise :: deserialize <uint32_t, myfixedclass, uint8_t> (data);

        if(lastop != 'B')
            throw "`deserialize` does not call the `bytewise` constructor of `myfixedclass` even if it is available.";

        if(otheralpha != 99)
            throw "`deserialize` does not return a `uint32_t` object consistent with what provided to `serialize`.";

        if(
            otherbeta.i != 4 ||
            otherbeta.k != 52 ||
            otherbeta.q[0].a[0] != 11 || otherbeta.q[0].a[1] != 22 || otherbeta.q[0].a[2] != 33 ||
            otherbeta.q[1].a[0] != 11 || otherbeta.q[1].a[1] != 22 || otherbeta.q[1].a[2] != 33 ||
            otherbeta.q[2].a[0] != 11 || otherbeta.q[2].a[1] != 22 || otherbeta.q[2].a[2] != 33 ||
            otherbeta.q[3].a[0] != 11 || otherbeta.q[3].a[1] != 22 || otherbeta.q[3].a[2] != 33
        )
            throw "`deserialize` does not return a `myfixedclass` object consistent with what provided to `serialize`.";

        if(othergamma != 'x')
            throw "`deserialize` does not return a `uint8_t` object consistent with what provided to `serialize`.";
    });

    $test("bytewise/serialize-deserialize-nonfixed", []
    {
        myotherclass alpha;
        myclass beta;
        uint32_t gamma = 88;

        std :: vector <uint8_t> data = bytewise :: serialize(alpha, beta, gamma);

        std :: vector <uint8_t> reference =
        {
            0, 0, 0, 0,
            1, 0, 0, 0,
            2, 0, 0, 0,
            3, 0, 0, 0,
            4, 0, 0, 0,
            9, 0, 0, 0,
            113,
                0, 0, 0, 0,
                1, 0, 0, 0,
                2, 0, 0, 0,
                3, 0, 0, 0,
                4, 0, 0, 0,
            15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
            3,
                1, 0, 0, 0,
                2, 0, 0, 0,
                3, 0, 0, 0,
                4, 0, 0, 0,
                5, 0, 0, 0,
                6, 0, 0, 0,
                7, 0, 0, 0,
                8, 0, 0, 0,
                9, 0, 0, 0,
                10, 0, 0, 0,
                11, 0, 0, 0,
                12, 0, 0, 0,
            2,
                13, 0, 0, 0,
                14, 0, 0, 0,
                15, 0, 0, 0,
                16, 0, 0, 0,
                17, 0, 0, 0,
                18, 0, 0, 0,
                19, 0, 0, 0,
                20, 0, 0, 0,
            88, 0, 0, 0
        };

        if(data != reference)
            throw "`serialize` does not produce the correct sequence of bytes when serializing `myotherclass, myclass, uint32_t`.";

        /*lastop = 'X';
        myclass otheritem = bytewise :: deserialize <myclass> (data);

        if(lastop != 'B')
            throw "`deserialize` does not call the `bytewise` constructor of `myclass` even if it is available.";

        if(
            otheritem.i != 9 ||
            otheritem.k != 'q' ||
            otheritem.q != std :: array <uint8_t, 16> {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0} ||
            otheritem.h != std :: array <std :: vector <std :: array <int32_t, 4>>, 2>
            {
                std :: vector <std :: array <int32_t, 4>>
                {
                    {1, 2, 3, 4},
                    {5, 6, 7, 8},
                    {9, 10, 11, 12}
                },
                std :: vector <std :: array <int32_t, 4>>
                {
                    {13, 14, 15, 16},
                    {17, 18, 19, 20}
                }
            }
        )
            throw "`deserialize` method does not return a `myclass` object consistent with what provided to `serialize`.";

        bool thrown = false;

        try
        {
            bytewise :: deserialize <myclass> (std :: vector <uint8_t> {});
        }
        catch(exception <buffer_error, out_of_range> &)
        {
            thrown = true;
        }

        if(!thrown)
            throw "Providing `deserialize` with an empty data buffer does not yield an exception.";*/
    });
};
