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
#include "syncset/syncset.labelset.hpp"
#include "syncset/syncset.listset.hpp"

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

    template <typename type> typename syncset <type> :: round syncset <type> :: sync()
    {
        round round;
        round.view.push_back(this->get <false> (prefix(hash(), 0)));
        return round;
    }

    template <typename type> typename syncset <type> :: round syncset <type> :: sync(const view & view)
    {
        round round;
        round.view.reserve(2 * view.size());

        size_t elements = 0;
        for(const auto & set : view)
            set.match([&](const listset & listset)
            {
                elements += listset.size();
            });

        round.add.reserve(elements);
        round.remove.reserve(elements);

        for(const auto & remote : view)
        {
            remote.match([&](const labelset & remote)
            {
                this->get <false> (remote.prefix()).match([&](const labelset & local)
                {
                    if(remote != local)
                    {
                        round.view.push_back(this->get <false> (remote.prefix().left()));
                        round.view.push_back(this->get <false> (remote.prefix().right()));
                    }
                }, [&](const listset & local)
                {
                    round.view.push_back(local);
                });
            }, [&](const listset & remote)
            {
                listset local = this->get <true> (remote.prefix()).template reinterpret <listset> ();
                if(remote != local)
                {
                    hash remotehash = remote.size() ? hash(remote[0]) : hash(bytewise{});
                    hash localhash = local.size() ? hash(local[0]) : hash(bytewise{});

                    size_t i = 0;
                    size_t j = 0;

                    while(i < remote.size() && j < local.size())
                    {
                        if(remotehash < localhash)
                        {
                            round.add.push_back(remote[i++]);
                            if(i < remote.size()) remotehash = hash(remote[i]);
                        }
                        else if(localhash < remotehash)
                        {
                            round.remove.push_back(local[j++]);
                            if(j < local.size()) localhash = hash(local[j]);
                        }
                        else
                        {
                            if(++i < remote.size()) remotehash = hash(remote[i]);
                            if(++j < local.size()) localhash = hash(local[j]);
                        }
                    }

                    while(i < remote.size())
                        round.add.push_back(remote[i++]);

                    while(j < local.size())
                        round.remove.push_back(local[j++]);

                    if(!(remote.dump()))
                        round.view.push_back(local);
                }
            });
        }

        return round;
    }

    // Private methods

    template <typename type> template <bool dump> typename syncset <type> :: set syncset <type> :: get(const prefix & prefix)
    {
        set response;
        navigator navigator(prefix, *(this->_root));

        for(; navigator && navigator.depth() <= prefix.depth(); navigator++);
        navigator--;

        navigator->match([&](const multiple & multiple)
        {
            if(dump || (multiple.size() <= settings :: thresholds :: list))
                response = listset(prefix, multiple, dump);
            else
                response = labelset(prefix, multiple);
        }, [&](const single & single)
        {
            bool match = true;

            for(size_t bit = navigator.depth(); bit < prefix.depth(); bit++)
                match &= (prefix[bit] == path(single.label())[bit]);

            if(match)
                response = listset(prefix, single, dump);
            else
                response = listset(prefix, empty(), dump);
        }, [&](const empty & empty)
        {
            response = listset(prefix, empty, dump);
        });

        return response;
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
