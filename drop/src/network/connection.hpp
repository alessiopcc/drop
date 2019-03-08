#ifndef __src__network__connection__hpp
#define __src__network__connection__hpp

// Includes

#include "connection.h"

namespace drop
{
    // connection

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
        this->setup <class send, true> ();

        try
        {
            if(this->_arc->_channelpair)
                this->lsendsync((*this->_arc->_channelpair).transmit.encrypt(message));
            else
                this->lsendsync(message);
        }
        catch(...)
        {
            this->release <class send> ();
            std :: rethrow_exception(std :: current_exception());
        }

        this->release <class send> ();
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ()) && !((sizeof...(types) == 1) && (... && connection :: constraints :: buffer <types> ()))> *> void connection :: sendsync(const types & ... message) const
    {
        this->sendsync(bytewise :: serialize(message...));
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> type connection :: receivesync() const
    {
        this->setup <class receive, true> ();

        type message;

        try
        {
            if(this->_arc->_channelpair)
                message = (*this->_arc->_channelpair).receive.decrypt(this->lreceivesync <typename channel :: traits :: encrypted <type> :: type> ());
            else
                message = this->lreceivesync <type> ();
        }
        catch(...)
        {
            this->release <class receive> ();
            std :: rethrow_exception(std :: current_exception());
        }

        this->release <class receive> ();
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

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> promise <void> connection :: sendasync(const type & message) const
    {
        connection connection = (*this);
        connection.setup <class send, false> ();

        try
        {
            co_await connection.lsendasync(message);
        }
        catch(...)
        {
            connection.release <class send> ();
            std :: rethrow_exception(std :: current_exception());
        }

        connection.release <class send> ();
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ()) && !((sizeof...(types) == 1) && (... && connection :: constraints :: buffer <types> ()))> *> promise <void> connection :: sendasync(const types & ... message) const
    {
        return this->sendasync(bytewise :: serialize(message...));
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> promise <type> connection :: receiveasync() const
    {
        connection connection = (*this);
        connection.setup <class receive, false> ();

        type message;

        try
        {
            message = co_await this->lreceiveasync <type> ();
        }
        catch(...)
        {
            connection.release <class receive> ();
            std :: rethrow_exception(std :: current_exception());
        }

        connection.release <class receive> ();
        co_return message;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !((sizeof...(types) == 1) && (... && connection :: constraints :: buffer <types> ()))> *> auto connection :: receiveasync() const
    {
        if constexpr ((... && bytewise :: constraints :: fixed <types> ()))
        {
            typedef std :: array <uint8_t, (... + bytewise :: traits :: size <types> ())> payload;
            typedef decltype(bytewise :: deserialize <types...> (std :: declval <payload> ())) rettype;

            return [&]() -> promise <rettype>
            {
                auto message = co_await this->receiveasync <payload> ();
                co_return bytewise :: deserialize <types...> (message);
            }();
        }
        else
        {
            typedef std :: vector <uint8_t> payload;
            typedef decltype(bytewise :: deserialize <types...> (std :: declval <payload> ())) rettype;

            return [&]() -> promise <rettype>
            {
                auto message = co_await this->receiveasync <payload> ();
                co_return bytewise :: deserialize <types...> (message);
            }();
        }
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> promise <void> connection :: send(const type & message) const
    {
        return this->sendasync(message);
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ()) && !((sizeof...(types) == 1) && (... && connection :: constraints :: buffer <types> ()))> *> promise <void> connection :: send(const types & ... message) const
    {
        return this->sendasync(message...);
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> promise <type> connection :: receive() const
    {
        return this->receiveasync <type> ();
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !((sizeof...(types) == 1) && (... && connection :: constraints :: buffer <types> ()))> *> auto connection :: receive() const
    {
        return this->receiveasync <types...> ();
    }

    // Private methods

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> void connection :: lsendsync(const type & message) const
    {
        streamer <class send> streamer(message);

        this->_arc->_socket.match([&](const auto & socket)
        {
            if(!streamer.stream(socket))
                exception <send_timeout> :: raise(this);
        });
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> type connection :: lreceivesync() const
    {
        type message;
        streamer <class receive> streamer(message);

        this->_arc->_socket.match([&](const auto & socket)
        {
            if(!streamer.stream(socket))
                exception <receive_timeout> :: raise(this);
        });

        return message;
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> promise <void> connection :: lsendasync(const type & message) const
    {
        bool completed;

        streamer <class send> streamer(message);

        this->_arc->_socket.match([&](const auto & socket)
        {
            completed = streamer.stream(socket);
        });

        if(!completed)
        {
            pool * binding = this->_arc->_guard([&](){return this->_arc->_pool;});
            promise <void> promise;

            this->_arc->_socket.match([&](const auto & socket)
            {
                promise = (binding ? *binding : pool :: system.get()).write(socket, streamer);
            });

            co_await promise;
        }
    }

    template <typename type, std :: enable_if_t <connection :: constraints :: buffer <type> ()> *> promise <type> connection :: lreceiveasync() const
    {
        bool completed;

        type message;
        streamer <class receive> streamer(message);

        this->_arc->_socket.match([&](const auto & socket)
        {
            completed = streamer.stream(socket);
        });

        if(!completed)
        {
            pool * binding = this->_arc->_guard([&](){return this->_arc->_pool;});
            promise <void> promise;

            this->_arc->_socket.match([&](const auto & socket)
            {
                promise = (binding ? *binding : pool :: system.get()).read(socket, streamer);
            });

            co_await promise;
        }

        co_return message;
    }

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
            this->block <blocking> ();

            if constexpr (std :: is_same <type, class send> :: value)
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
        });
    }

    template <typename type> void connection :: release() const
    {
        this->_arc->_guard([&]()
        {
            if constexpr (std :: is_same <type, class send> :: value)
                this->_arc->_locks.send = false;
            else
                this->_arc->_locks.receive = false;
        });
    }
};

#endif
