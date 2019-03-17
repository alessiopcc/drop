#ifndef __src__async__promise__hpp
#define __src__async__promise__hpp

// Includes

#include "promise.h"

namespace drop
{
    // promise

    // Constraints

    template <typename type> template <typename... dummy> constexpr bool promise <type> :: constraints :: resolve()
    {
        if constexpr (std :: is_same <type, void> :: value)
            return sizeof...(dummy) == 0;
        else
        {
            if constexpr (sizeof...(dummy) == 1)
                return std :: is_same <dummy..., type> :: value;
            else
                return false;
        }
    }

    // Constructors

    template <typename type> promise <type> :: promise() : _arc(std :: make_shared <arc> ())
    {
    }

    // Awaitable interface

    template <typename type> bool promise <type> :: await_ready() const
    {
        return this->_arc->_guard([&]()
        {
            return this->_arc->_status;
        });
    }

    template <typename type> void promise <type> :: await_suspend(std :: experimental :: coroutine_handle <> handle) const
    {
        this->_arc->_guard([&]()
        {
            if(!(this->_arc->_handles))
                this->_arc->_handles = handle;
            else if(this->_arc->_handles.template is <std :: experimental :: coroutine_handle <>> ())
            {
                auto old = this->_arc->_handles.template reinterpret <std :: experimental :: coroutine_handle <>> ();
                this->_arc->_handles = std :: vector <std :: experimental :: coroutine_handle <>> {old, handle};
            }
            else
                this->_arc->_handles.template reinterpret <std :: vector <std :: experimental :: coroutine_handle <>>> ().push_back(handle);
        });
    }

    template <typename type> auto promise <type> :: await_resume() const
    {
        this->_arc->_guard([&]()
        {
            this->_arc->_resumed = true;
        });

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

    template <typename type> template <typename... vtype, std :: enable_if_t <promise <type> :: constraints :: template resolve <vtype...> ()> *> void promise <type> :: resolvehard(const vtype & ... value) const
    {
        this->resolve <true> (value...);
    }

    template <typename type> template <typename... vtype, std :: enable_if_t <promise <type> :: constraints :: template resolve <vtype...> ()> *> void promise <type> :: resolvesoft(const vtype & ... value) const
    {
        this->resolve <false> (value...);
    }

    template <typename type> template <typename... vtype, std :: enable_if_t <promise <type> :: constraints :: template resolve <vtype...> ()> *> void promise <type> :: resolve(const vtype & ... value) const
    {
        this->resolvehard(value...);
    }

    template <typename type> void promise <type> :: rejecthard(const std :: exception_ptr & exception) const
    {
        this->reject <true> (exception);
    }

    template <typename type> void promise <type> :: rejectsoft(const std :: exception_ptr & exception) const
    {
        this->reject <false> (exception);
    }

    template <typename type> void promise <type> :: reject(const std :: exception_ptr & exception) const
    {
        this->rejecthard(exception);
    }

    template <typename type> template <typename etype> void promise <type> :: rejecthard(const etype & exception) const
    {
        this->reject <true> (exception);
    }

    template <typename type> template <typename etype> void promise <type> :: rejectsoft(const etype & exception) const
    {
        this->reject <false> (exception);
    }

    template <typename type> template <typename etype> void promise <type> :: reject(const etype & exception) const
    {
        this->rejecthard(exception);
    }

    // Private methods

    template <typename type> template <bool hard, typename... vtype, std :: enable_if_t <promise <type> :: constraints :: template resolve <vtype...> ()> *> void promise <type> :: resolve(const vtype & ... value) const
    {
        std :: shared_ptr <arc> arc = this->_arc;

        handles handles;

        arc->_guard([&]()
        {
            if(arc->_status)
            {
                if constexpr (hard)
                    exception <bad_access, already_resolved> :: raise(this);
            }
            else
            {
                if constexpr (std :: is_same <type, void> :: value)
                    arc->_status = null();
                else
                    [&](const type & value)
                    {
                        arc->_status = value;
                    }(value...);

                std :: swap(handles, arc->_handles);
            }
        });

        flush(handles);
    }

    template <typename type> template <bool hard> void promise <type> :: reject(const std :: exception_ptr & exception) const
    {
        std :: shared_ptr <arc> arc = this->_arc;

        handles handles;

        arc->_guard([&]()
        {
            if(arc->_status)
            {
                if constexpr (hard)
                    drop :: exception <bad_access, already_resolved> :: raise(this);
            }
            else
            {
                arc->_status = exception;
                std :: swap(handles, arc->_handles);
            }
        });

        flush(handles);
    }

    template <typename type> template <bool hard, typename etype> void promise <type> :: reject(const etype & exception) const
    {
        this->reject <hard> (std :: make_exception_ptr(exception));
    }

    // Static private methods

    template <typename type> void promise <type> :: flush(handles & handles)
    {
        handles.match([](std :: experimental :: coroutine_handle <> & handle)
        {
            handle.resume();
        }, [](std :: vector <std :: experimental :: coroutine_handle <>> & handles)
        {
            for(auto & handle : handles)
                handle.resume();
        });
    }

    // promise_base

    // Object

    template <typename type> inline auto promise <type> :: promise_base :: get_return_object()
    {
        return this->_promise;
    }

    // Suspends

    template <typename type> inline auto promise <type> :: promise_base :: initial_suspend()
    {
        return std :: experimental :: suspend_never();
    }

    template <typename type> inline auto promise <type> :: promise_base :: final_suspend()
    {
        return std :: experimental :: suspend_never();
    }

    // Exceptions

    template <typename type> inline void promise <type> :: promise_base :: unhandled_exception()
    {
        this->_promise.reject(std :: current_exception());
    }

    // promise <void> :: promise_type

    // Returns

    inline void promise <void> :: promise_type :: return_void()
    {
        this->_promise.resolve();
    }

    // promise <type> :: promise_type

    template <typename type> inline void promise <type> :: promise_type :: return_value(const type & value)
    {
        this->_promise.resolve(value);
    }

    // arc

    // Constructors

    template <typename type> promise <type> :: arc :: arc() : _resumed(false)
    {
    }

    // Destructor

    template <typename type> promise <type> :: arc :: ~arc()
    {
        if(this->_status.template is <std :: exception_ptr> () && !(this->_resumed))
        {
            std :: cerr << "Deleting rejected `promise` that was never handled: ";

            try
            {
                std :: rethrow_exception(this->_status.template reinterpret <std :: exception_ptr> ());
            }
            catch(const std :: exception & exception)
            {
                std :: cerr << exception.what();
            }
            catch(const char * exception)
            {
                std :: cerr << exception;
            }
            catch(...)
            {
                std :: cerr << "(unknown exception)";
            }

            std :: cerr << std :: endl;
            std :: terminate();
        }
    }
};

#endif
