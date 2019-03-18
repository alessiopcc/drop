#ifndef __src__data__syncset__syncset__multiple__hpp
#define __src__data__syncset__syncset__multiple__hpp

// Includes

#include "../syncset.h"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: multiple :: multiple(const node & left, const node & right) : _left(new node(left)), _right(new node(right))
    {
        this->refresh();
    }

    template <typename type> syncset <type> :: multiple :: multiple(const multiple & that) : _label(that._label), _size(that._size), _left(new node(*that._left)), _right(new node(*that._right))
    {
    }

    template <typename type> syncset <type> :: multiple :: multiple(multiple && that) : _label(that._label), _size(that._size), _left(that._left), _right(that._right)
    {
        that._left = nullptr;
        that._right = nullptr;
    }

    // Destructor

    template <typename type> syncset <type> :: multiple :: ~multiple()
    {
        if(this->_left)
        {
            delete this->_left;
            delete this->_right;
        }
    }

    // Getters

    template <typename type> const hash & syncset <type> :: multiple :: label() const
    {
        return this->_label;
    }

    template <typename type> const size_t & syncset <type> :: multiple :: size() const
    {
        return this->_size;
    }

    template <typename type> typename syncset <type> :: node * syncset <type> :: multiple :: left() const
    {
        return this->_left;
    }

    template <typename type> typename syncset <type> :: node * syncset <type> :: multiple :: right() const
    {
        return this->_right;
    }

    // Methods

    template <typename type> void syncset <type> :: multiple :: refresh()
    {
        this->_size = 0;

        this->_left->match([&](const single &)
        {
            this->_size++;
        }, [&](const multiple & left)
        {
            this->_size += left._size;
        });

        this->_right->match([&](const single &)
        {
            this->_size++;
        }, [&](const multiple & right)
        {
            this->_size += right._size;
        });

        this->_left->match([&](const auto & left)
        {
            this->_right->match([&](const auto & right)
            {
                this->_label = hash(left.label(), right.label());
            });
        });
    }

    template <typename type> bool syncset <type> :: multiple :: pullable()
    {
        return  (this->_left->template is <single> () && this->_right->template is <empty> ()) || (this->_left->template is <empty> () && this->_right->template is <single> ());
    }

    template <typename type> typename syncset <type> :: single syncset <type> :: multiple :: pull()
    {
        return this->_left->template is <single> () ? this->_left->template reinterpret <single> () : this->_right->template reinterpret <single> ();
    }

    template <typename type> template <typename lambda> void syncset <type> :: multiple :: traverse(lambda && callback) const
    {
        this->_left->match([&](const auto & left)
        {
            left.traverse(callback);
        });

        this->_right->match([&](const auto & right)
        {
            right.traverse(callback);
        });
    }

    // Operators

    template <typename type> typename syncset <type> :: multiple & syncset <type> :: multiple :: operator = (const multiple & rho)
    {
        this->~multiple();

        this->_label = rho._label;
        this->_size = rho._size;
        this->_left = new node(*rho._left);
        this->_right = new node(*rho._right);

        return (*this);
    }


    template <typename type> typename syncset <type> :: multiple & syncset <type> :: multiple :: operator = (multiple && rho)
    {
        this->~multiple();

        this->_label = rho._label;
        this->_size = rho._size;
        this->_left = rho._left;
        this->_right = rho._right;

        rho._left = nullptr;
        rho._right = nullptr;

        return (*this);
    }
};

#endif
