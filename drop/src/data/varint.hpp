#ifndef __src__data__varint__hpp
#define __src__data__varint__hpp

// Includes

#include "varint.h"
#include "drop/bytewise/bytewise.hpp"
#include "drop/concept/expression.hpp"

namespace drop
{
    // Constraints

    template <typename lambda> constexpr bool varint :: constraints :: visitor()
    {
        typedef std :: array <uint8_t, 1> onebyte;
        typedef std :: array <uint8_t, 2> twobytes;
        typedef std :: array <uint8_t, 4> fourbytes;

        return $expression($type(lambda)($type(const onebyte &))) &&
               $expression($type(lambda)($type(const twobytes &))) &&
               $expression($type(lambda)($type(const fourbytes &)));
    }

    // Bytewise

    template <typename vtype> void varint :: accept(bytewise :: reader <vtype> & reader) const
    {
        this->visit([&](const auto & buffer)
        {
            reader.visit(buffer);
        });
    }

    template <typename vtype> void varint :: accept(bytewise :: writer <vtype> & writer)
    {
        std :: array <uint8_t, 4> buffer;
        writer.visit(buffer[0]);

        switch(size(buffer[0]))
        {
            case 2:
            {
                writer.visit(buffer[1]);
                break;
            }
            case 4:
            {
                writer.visit(buffer[1]).visit(buffer[2]).visit(buffer[3]);
                break;
            }
        }

        (*this) = varint(buffer.data());
    }

    // Methods

    template <typename lambda, std :: enable_if_t <varint :: constraints :: visitor <lambda> ()> *> void varint :: visit(lambda && visitor) const
    {
        if(this->_value < 128)
        {
            std :: array <uint8_t, 1> buffer;
            buffer[0] = (uint8_t) this->_value;

            visitor(buffer);
        }
        else if(this->_value < 16384)
        {
            std :: array <uint8_t, 2> buffer;
            buffer[0] = (uint8_t)(this->_value >> 8) | 0x80;
            buffer[1] = (uint8_t)(this->_value);

            visitor(buffer);
        }
        else
        {
            std :: array <uint8_t, 4> buffer;
            buffer[0] = (uint8_t)(this->_value >> 24) | 0xc0;
            buffer[1] = (uint8_t)(this->_value >> 16);
            buffer[2] = (uint8_t)(this->_value >> 8);
            buffer[3] = (uint8_t)(this->_value);

            visitor(buffer);
        }
    }
};

#endif
