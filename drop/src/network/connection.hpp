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
        this->_arc->_guard([&]()
        {
            if(this->_arc->_locks.send)
                exception <send_failed, send_locked> :: raise(this);

            this->block <true> ();
            this->_arc->_locks.send = true;
        });

        this->_arc->_socket.match([&](const auto & socket)
        {
            streamer <send> streamer(message);

            if(!streamer.stream(socket))
                exception <send_timeout> :: raise(this);
        });
    }

    template <typename type, std :: enable_if_t <(bytewise :: constraints :: serializable <type> ()) && !(connection :: constraints :: buffer <type> ())> *> void connection :: sendsync(const type & message) const
    {
        this->sendsync(bytewise :: serialize(message));
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> type connection :: receivesync() const
    {
        this->_arc->_guard([&]()
        {
            if(this->_arc->_locks.receive)
                exception <receive_failed, receive_locked> :: raise(this);

            this->block <true> ();
            this->_arc->_locks.receive = true;
        });

        type message;

        this->_arc->_socket.match([&](const auto & socket)
        {
            streamer <receive> streamer(message);

            if(!streamer.stream(socket))
                exception <receive_timeout> :: raise(this);
        });

        return message;
    }

    template <typename type, std :: enable_if_t <(bytewise :: constraints :: deserializable <type> ()) && !(connection :: constraints :: buffer <type> ())> *> type connection :: receivesync() const
    {
        if constexpr (bytewise :: constraints :: fixed <type> ())
        {
            auto message = this->receivesync <std :: array <uint8_t, bytewise :: traits :: size <type> ()>> ();
            return bytewise :: deserialize <type> (message);
        }
        else
        {
            auto message = this->receivesync <std :: vector <uint8_t>> ();
            return bytewise :: deserialize <type> (message);
        }
    }

    // Private methods

    template <bool value> void connection :: block() const
    {
        if(this->_arc->_cache.blocking != value)
        {
            if(this->_arc->_locks.send || this->_arc->_locks.receive)
                exception <synchrony_locked> :: raise(this);

            this->_arc->_socket.match([](auto & socket)
            {
                socket.template set <blocking> (value);
            });

            this->_arc->_cache.blocking = value;
        }
    }
};

#endif
