#ifndef __src__data__varint__hpp
#define __src__data__varint__hpp

// Includes

#include "varint.h"
#include "bytewise/bytewise.hpp"

namespace drop
{
    // Methods

    template <typename vtype> void varint :: accept(bytewise :: reader <vtype> & reader) const
    {
        if(this->_value < 128)
        {
            std :: array <uint8_t, 1> buffer;
            buffer[0] = (uint8_t) this->_value;
            reader.visit(buffer);
        }
        else if(this->_value < 16384)
        {
            std :: array <uint8_t, 2> buffer;
            buffer[0] = (uint8_t)(this->_value >> 8) | 0x80;
            buffer[1] = (uint8_t)(this->_value);
            reader.visit(buffer);
        }
        else
        {
            std :: array <uint8_t, 4> buffer;
            buffer[0] = (uint8_t)(this->_value >> 24) | 0xc0;
            buffer[1] = (uint8_t)(this->_value >> 16);
            buffer[2] = (uint8_t)(this->_value >> 8);
            buffer[3] = (uint8_t)(this->_value);
            reader.visit(buffer);
        }
    }

    template <typename vtype> void varint :: accept(bytewise :: writer <vtype> & writer)
    {
        std :: array <uint8_t, 4> buffer;

        writer.visit(buffer[0]);
        if(buffer[0] & 0x80)
        {
            writer.visit(buffer[1]);

            if(buffer[0] & 0x40)
            {
                writer.visit(buffer[2]).visit(buffer[3]);

                this->_value = ((uint32_t)(buffer[0] & 0x3f) << 24) | ((uint32_t)(buffer[1]) << 16) | ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
            }
            else
                this->_value = ((uint32_t)(buffer[0] & 0x7f) << 8) | (uint32_t)(buffer[1]);
        }
        else
            this->_value = buffer[0];
    }
};

#endif
