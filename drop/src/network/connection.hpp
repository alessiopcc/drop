#ifndef __src__network__connection__hpp
#define __src__network__connection__hpp

// Includes

#include "connection.h"

namespace drop
{
    // Constraints

    template <typename type> constexpr bool connection :: constraints :: buffer()
    {
        if constexpr (stltraits :: array <type> :: value)
            return std :: is_same <typename stltraits :: array <type> :: type, uint8_t> :: value;
        else
            return std :: is_same <type, std :: vector <uint8_t>> :: value;
    }

    // Methods

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> void connection :: sendsync(const type & message) const
    {
        this->setup <send, true> ();

        try
        {
            this->_arc->_socket.match([&](const auto & socket)
            {
                streamer <send> streamer(message);

                if(!streamer.stream(socket))
                    exception <send_timeout> :: raise(this);
            });
        }
        catch(...)
        {
            this->release <send> ();
            std :: rethrow_exception(std :: current_exception());
        }

        this->release <send> ();
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ()) && !((sizeof...(types) == 1) && (... && connection :: constraints :: buffer <types> ()))> *> void connection :: sendsync(const types & ... message) const
    {
        this->sendsync(bytewise :: serialize(message...));
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> type connection :: receivesync() const
    {
        this->setup <receive, true> ();
        type message;

        try
        {
            this->_arc->_socket.match([&](const auto & socket)
            {
                streamer <receive> streamer(message);

                if(!streamer.stream(socket))
                    exception <receive_timeout> :: raise(this);
            });
        }
        catch(...)
        {
            this->release <receive> ();
            std :: rethrow_exception(std :: current_exception());
        }

        this->release <receive> ();
        return message;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !((sizeof...(types) == 1) && (... && connection :: constraints :: buffer <types> ()))> *> auto connection :: receivesync() const
    {
        if constexpr ((... && bytewise :: constraints :: fixed <types> ()))
        {
            auto message = this->receivesync <std :: array <uint8_t, (... + bytewise :: traits :: size <types> ())>> ();
            return bytewise :: deserialize <types...> (message);
        }
        else
        {
            auto message = this->receivesync <std :: vector <uint8_t>> ();
            return bytewise :: deserialize <types...> (message);
        }
    }

    // Private methods

    template <bool value> void connection :: block() const
    {
        if(this->_arc->_cache.blocking != value)
        {
            if(this->_arc->_locks.send || this->_arc->_locks.receive)
                exception <synchrony_locked> :: raise(this);

            this->_arc->_socket.match([](const auto & socket)
            {
                socket.template set <blocking> (value);
            });

            this->_arc->_cache.blocking = value;
        }
    }

    template <typename type, bool blocking> void connection :: setup() const
    {
        this->_arc->_guard([&]()
        {
            if constexpr (std :: is_same <type, send> :: value)
            {
                if(this->_arc->_locks.send)
                    exception <send_failed, send_locked> :: raise(this);

                this->_arc->_locks.send = true;
            }
            else
            {
                if(this->_arc->_locks.receive)
                    exception <receive_failed, receive_locked> :: raise(this);

                this->_arc->_locks.receive = true;
            }

            this->block <blocking> ();
        });
    }

    template <typename type> void connection :: release() const
    {
        this->_arc->_guard([&]()
        {
            if constexpr (std :: is_same <type, send> :: value)
                this->_arc->_locks.send = false;
            else
                this->_arc->_locks.receive = false;
        });
    }
};

#endif
