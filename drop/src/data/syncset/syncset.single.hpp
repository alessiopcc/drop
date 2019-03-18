#ifndef __src__data__syncset__syncset__single__hpp
#define __src__data__syncset__syncset__single__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: single :: single(const type & element) : _label(element), _element(element)
    {
    }

    // Getters

    template <typename type> const hash & syncset <type> :: single :: label() const
    {
        return this->_label;
    }

    template <typename type> const type & syncset <type> :: single :: element() const
    {
        return this->_element;
    }

    // Methods

    template <typename type> typename syncset <type> :: empty syncset <type> :: single :: empty()
    {
        return syncset <type> :: empty();
    }

    template <typename type> typename syncset <type> :: multiple syncset <type> :: single :: push(const navigation & child)
    {
        return (child == left) ? multiple(*this, (class empty){}) : multiple((class empty){}, *this);
    }

    template <typename type> template <typename lambda> void syncset <type> :: single :: traverse(lambda && callback) const
    {
        callback(this->_element);
    }
};

#endif
