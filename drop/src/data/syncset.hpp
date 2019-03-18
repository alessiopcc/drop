#ifndef __src__data__syncset__hpp
#define __src__data__syncset__hpp

// Includes

#include "syncset.h"
#include "syncset/syncset.path.hpp"
#include "syncset/syncset.prefix.hpp"
#include "syncset/syncset.navigator.hpp"
#include "syncset/syncset.empty.hpp"
#include "syncset/syncset.single.hpp"
#include "syncset/syncset.multiple.hpp"

namespace drop
{
    // Constructors

    template <typename type> syncset <type> :: syncset() : _root(new node(empty()))
    {
    }

    template <typename type> syncset <type> :: syncset(const syncset & that) : _root(new node(*(that._root)))
    {
    }

    template <typename type> syncset <type> :: syncset(syncset && that) : _root(that._root)
    {
        that._root = nullptr;
    }

    // Destructor

    template <typename type> syncset <type> :: ~syncset()
    {
        if(this->_root)
            delete this->_root;
    }

    // Methods

    template <typename type> void syncset <type> :: add(const type & element)
    {
        path path(element);
        navigator navigator(path, *(this->_root));

        optional <class path> push;

        for(; navigator; navigator++)
        {
            bool collision = false;

            navigator->match([&](empty & empty)
            {
                (*navigator) = empty.fill(element);
            }, [&](single & single)
            {
                if(single.element() == element)
                    collision = true;
                else
                {
                    if(!push)
                        push.template emplace <class path> (single.element());

                    (*navigator) = single.push((*push)[navigator.depth()]);
                }
            });

            if(collision)
                return;
        }

        navigator--;

        while(navigator.depth() > 0)
        {
            navigator--;
            navigator->template reinterpret <multiple> ().refresh();
        }
    }

    template <typename type> void syncset <type> :: remove(const type & element)
    {
        path path(element);
        navigator navigator(path, *(this->_root));

        bool found = false;
        for(; navigator; navigator++)
        {
            navigator->match([&](single & single)
            {
                if(single.element() == element)
                {
                    (*navigator) = single.empty();
                    found = true;
                }
            });
        }

        if(!found)
            return;

        navigator--;

        while(navigator.depth() > 0)
        {
            navigator--;

            if(navigator->template reinterpret <multiple> ().pullable())
                (*navigator) = navigator->template reinterpret <multiple> ().pull();
            else
                break;
        }

        while(navigator.depth() > 0)
        {
            navigator--;
            navigator->template reinterpret <multiple> ().refresh();
        }
    }

    template <typename type> bool syncset <type> :: find(const type & element)
    {
        path path(element);
        optional <bool> found;

        for(navigator navigator(path, *(this->_root)); !found; navigator++)
        {
            navigator->match([&](empty & empty)
            {
                found = false;
            }, [&](single & single)
            {
                if(single.element() == element)
                    found = true;
                else
                    found = false;
            });
        }

        return (*found);
    }

    // Operators

    template <typename type> syncset <type> & syncset <type> :: operator = (const syncset <type> & rho)
    {
        this->~syncset();
        this->_root = new node(*(rho._root));
        return (*this);
    }

    template <typename type> syncset <type> & syncset <type> :: operator = (syncset <type> && rho)
    {
        this->~syncset();
        this->_root = rho._root;
        rho._root = nullptr;
        return (*this);
    }
};

#endif
