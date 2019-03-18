#ifndef __src__data__syncset__syncset__prefix__hpp
#define __src__data__syncset__syncset__prefix__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: prefix :: prefix(const path & path, const size_t & depth) : syncset <type> :: path(path), _depth(depth)
    {
    }

    template <typename type> syncset <type> :: prefix :: prefix(bytewise) : path(bytewise{})
    {
    }

    // Bytewise

    template <typename type> template <typename vtype> void syncset <type> :: prefix :: accept(bytewise :: reader <vtype> & reader) const
    {
        reader.visit(this->_depth);
        for(size_t byte = 0; byte < (this->_depth + 7) / 8; byte++)
            reader.visit(((hash &) *this)[byte]);
    }

    template <typename type> template <typename vtype> void syncset <type> :: prefix :: accept(bytewise :: writer <vtype> & writer)
    {
        writer.visit(this->_depth);
        for(size_t byte = 0; byte < (this->_depth + 7) / 8; byte++)
            writer.visit(((hash &) *this)[byte]);
    }

    // Getters

    template <typename type> size_t syncset <type> :: prefix :: depth() const
    {
        return this->_depth;
    }

    // Methods

    template <typename type> typename syncset <type> :: prefix syncset <type> :: prefix :: left() const
    {
        prefix left = (*this);

        size_t byte = this->_depth / 8;
        size_t bit = this->_depth % 8;

        left._hash[byte] &= (~(1 << (7 - bit)));
        left._depth++;

        return left;
    }

    template <typename type> typename syncset <type> :: prefix syncset <type> :: prefix :: right() const
    {
        prefix right = (*this);

        size_t byte = this->_depth / 8;
        size_t bit = this->_depth % 8;

        right._hash[byte] |= (1 << (7 - bit));
        right._depth++;

        return right;
    }
};

#endif
