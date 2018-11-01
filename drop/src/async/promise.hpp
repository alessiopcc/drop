#ifndef __src__async__promise__hpp
#define __src__async__promise__hpp

// Includes

#include "promise.h"

namespace drop
{
    // promise

    // Constructors

    template <typename type> promise <type> :: promise() : _arc(std :: make_shared <arc> ())
    {
    }

    // Awaitable interface

    template <typename type> bool promise <type> :: await_ready() const
    {
        bool response;

        this->_arc->_mutex.lock();
        response = this->_arc->_status;
        this->_arc->_mutex.unlock();

        return response;
    }

    template <typename type> void promise <type> :: await_suspend(std :: experimental :: coroutine_handle <> handle) const
    {
        this->_arc->_mutex.lock();

        if(!(this->_arc->_handles))
            this->_arc->_handles = handle;
        else if(this->_arc->_handles.template is <std :: experimental :: coroutine_handle <>> ())
        {
            auto old = this->_arc->_handles.template reinterpret <std :: experimental :: coroutine_handle <>> ();
            this->_arc->_handles = std :: vector <std :: experimental :: coroutine_handle <>> {old, handle};
        }
        else
            this->_arc->_handles.template reinterpret <std :: vector <std :: experimental :: coroutine_handle <>>> ().push_back(handle);

        this->_arc->_mutex.unlock();
    }

    template <typename type> auto promise <type> :: await_resume() const
    {
        if(this->_arc->_status.template is <typename arc :: storetype> ())
        {
            if constexpr (std :: is_same <type, void> :: value)
                return;
            else
                return this->_arc->_status.template reinterpret <type> ();
        }
        else
            std :: rethrow_exception(this->_arc->_status.template reinterpret <std :: exception_ptr> ());
    }

    // Methods

    template <typename type> template <typename... dummy, std :: enable_if_t <sizeof...(dummy) == 0 && std :: is_same <type, void> :: value> *> void promise <type> :: resolve(dummy...) const
    {
        this->_mutex.lock();

        if(this->_arc->_status)
        {
            this->_mutex.unlock();
            exception <bad_access, already_resolved> :: raise(this);
        }
        else
        {
            this->_arc->_status = null();
            this->_arc->_handles.match([](std :: experimental :: coroutine_handle <> & handle)
            {
                handle.resume();
            }, [](std :: vector <std :: experimental :: coroutine_handle <>> & handles)
            {
                for(auto & handle : handles)
                    handle.resume();
            });

            this->_arc->_handles.erase();
        }

        this->_mutex.unlock();
    }

    // promise_type

    // Object

    template <typename type> inline auto promise <type> :: promise_type :: get_return_object()
    {
        return this->_promise;
    }

    // Suspends

    template <typename type> inline auto promise <type> :: promise_type :: initial_suspend()
    {
        return std :: experimental :: suspend_never();
    }

    template <typename type> inline auto promise <type> :: promise_type :: final_suspend()
    {
        return std :: experimental :: suspend_never();
    }

    // Exceptions

    template <typename type> inline void promise <type> :: promise_type :: unhandled_exception()
    {
        this->_promise.reject(std :: current_exception());
    }

    // Returns

    template <typename type> template <typename... dummy, std :: enable_if_t <sizeof...(dummy) == 0 && std :: is_same <type, void> :: value> *> inline void promise <type> :: promise_type :: return_void(dummy...)
    {
        this->_promise.resolve();
    }

    template <typename type> template <typename vtype, std :: enable_if_t <std :: is_same <vtype, type> :: value && !(std :: is_same <type, void> :: value)> *> inline void promise <type> :: promise_type :: return_value(const vtype & value)
    {
        this->_promise.resolve(value);
    }
};

#endif
