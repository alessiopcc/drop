#ifndef __src__data__syncset__syncset__labelset__hpp
#define __src__data__syncset__syncset__labelset__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: labelset :: labelset(const class prefix & prefix, const multiple & node) : _prefix(prefix), _label(node.label())
    {
    }

    // Getters

    template <typename type> const typename syncset <type> :: prefix & syncset <type> :: labelset :: prefix() const
    {
        return this->_prefix;
    }

    template <typename type> const hash & syncset <type> :: labelset :: label() const
    {
        return this->_label;
    }

    // Operators

    template <typename type> bool syncset <type> :: labelset :: operator == (const labelset & rho) const
    {
        return (this->_label == rho._label);
    }

    template <typename type> bool syncset <type> :: labelset :: operator != (const labelset & rho) const
    {
        return !((*this) == rho);
    }
};

#endif
