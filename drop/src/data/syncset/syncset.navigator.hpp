#ifndef __src__data__syncset__syncset__navigator__hpp
#define __src__data__syncset__syncset__navigator__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: navigator :: navigator(const path & path, node & root) : _path(path), _depth(0), _trace{&root}
    {
    }

    // Getters

    template <typename type> const size_t & syncset <type> :: navigator :: depth() const
    {
        return this->_depth;
    }

    template <typename type> typename syncset <type> :: navigation syncset <type> :: navigator :: next() const
    {
        return this->_path[this->_depth];
    }

    // Operators

    template <typename type> typename syncset <type> :: node & syncset <type> :: navigator :: operator * ()
    {
        return *(this->_trace[this->_depth]);
    }

    template <typename type> typename syncset <type> :: node * syncset <type> :: navigator :: operator -> ()
    {
        return this->_trace[this->_depth];
    }

    template <typename type> typename syncset <type> :: navigator & syncset <type> :: navigator :: operator ++ ()
    {
        this->_trace[this->_depth]->match([&](multiple & node)
        {
            this->_trace[this->_depth + 1] = (this->next() == left) ? node.left() : node.right();
        }, [&](auto &)
        {
            this->_trace[this->_depth + 1] = nullptr;
        });

        this->_depth++;
        return (*this);
    }

    template <typename type> typename syncset <type> :: navigator & syncset <type> :: navigator :: operator ++ (int)
    {
        return ++(*this);
    }

    template <typename type> typename syncset <type> :: navigator & syncset <type> :: navigator :: operator -- ()
    {
        this->_depth--;
        return (*this);
    }

    template <typename type> typename syncset <type> :: navigator & syncset <type> :: navigator :: operator -- (int)
    {
        return --(*this);
    }

    // Casting

    template <typename type> syncset <type> :: navigator :: operator bool () const
    {
        return this->_trace[this->_depth];
    }
};

#endif
