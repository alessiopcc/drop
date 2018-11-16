#ifndef __src__crypto__shorthash__hpp
#define __src__crypto__shorthash__hpp

// Includes

#include "shorthash.h"

namespace drop
{
    // shorthash

    // Operators

    template <typename type, std :: enable_if_t <bytewise :: constraints :: oneshot <type> ()> *> size_t shorthash :: operator () (const type & item) const noexcept
    {
        bytewise :: read(*(this->_oneshot), item);
        return this->_oneshot->value();
    }

    template <typename type, std :: enable_if_t <bytewise :: constraints :: readable <type, shorthash :: buffer> () && !(bytewise :: constraints :: oneshot <type> ())> *> size_t shorthash :: operator () (const type & item) const noexcept
    {
        bytewise :: read(*(this->_buffer), item);
        return this->_buffer->finalize();
    }
};

#endif
