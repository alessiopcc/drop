#ifndef __src__data__syncset__syncset__listset__hpp
#define __src__data__syncset__syncset__listset__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: listset :: listset(const class prefix & prefix, const empty & node, const bool & dump) : _prefix(prefix), _dump(dump)
    {
    }

    template <typename type> syncset <type> :: listset :: listset(const class prefix & prefix, const single & node, const bool & dump) : std :: vector <type> {node.element()}, _prefix(prefix), _dump(dump)
    {
    }

    template <typename type> syncset <type> :: listset :: listset(const class prefix & prefix, const multiple & node, const bool & dump) : _prefix(prefix), _dump(dump)
    {
        this->reserve(node.size());
        node.traverse([&](const type & element)
        {
            this->push_back(element);
        });
    }

    // Getters

    template <typename type> const typename syncset <type> :: prefix & syncset <type> :: listset :: prefix() const
    {
        return this->_prefix;
    }

    template <typename type> const bool & syncset <type> :: listset :: dump() const
    {
        return this->_dump;
    }
};

#endif
