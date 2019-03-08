// Includes

#include "connection.hpp"

namespace drop
{
    // connection

    // Constructors

    connection :: connection(const tcp :: socket & socket) : _arc(std :: make_shared <arc> ())
    {
        this->_arc->_socket = socket;
        this->_arc->_cache.blocking = socket.get <blocking> ();

        this->_arc->_locks.send = false;
        this->_arc->_locks.receive = false;

        this->_arc->_pool = nullptr;
    }

    // Methods

    void connection :: securesync(const keyexchanger & local, const class keyexchanger :: publickey & remote) const
    {
        this->setup <class send, true> ();
        this->setup <class receive, true> ();

        try
        {
            auto keypair = local.exchange(remote);
            auto transmitnonce = channel :: nonce :: random();

            this->lsendsync(bytewise :: serialize(transmitnonce));
            auto receivenonce = bytewise :: deserialize <class channel :: nonce> (this->lreceivesync <std :: array <uint8_t, bytewise :: traits :: size <class channel :: nonce> ()>> ());

            this->_arc->_channelpair.emplace <arc :: channelpair> (keypair.receive, receivenonce, keypair.transmit, transmitnonce);
        }
        catch(...)
        {
            this->release <class send> ();
            this->release <class receive> ();

            std :: rethrow_exception(std :: current_exception());
        }

        this->release <class send> ();
        this->release <class receive> ();
    }

    promise <void> connection :: secureasync(const keyexchanger & local, const class keyexchanger :: publickey & remote) const
    {
        connection connection = (*this);

        connection.setup <class send, false> ();
        connection.setup <class receive, false> ();

        try
        {
            auto keypair = local.exchange(remote);
            auto transmitnonce = channel :: nonce :: random();

            co_await connection.lsendasync(bytewise :: serialize (transmitnonce));
            auto receivenonce = bytewise :: deserialize <class channel :: nonce> (co_await connection.lreceiveasync <std :: array <uint8_t, bytewise :: traits :: size <class channel :: nonce> ()>> ());

            connection._arc->_channelpair.emplace <arc :: channelpair> (keypair.receive, receivenonce, keypair.transmit, transmitnonce);
        }
        catch(...)
        {
            connection.release <class send> ();
            connection.release <class receive> ();

            std :: rethrow_exception(std :: current_exception());
        }

        connection.release <class send> ();
        connection.release <class receive> ();
    }

    promise <void> connection :: secure(const keyexchanger & local, const class keyexchanger :: publickey & remote) const
    {
        return this->secureasync(local, remote);
    }

    void connection :: bind(pool & pool) const
    {
        this->_arc->_guard([&]()
        {
            this->_arc->_pool = &pool;
        });
    }

    void connection :: unbind() const
    {
        this->_arc->_guard([&]()
        {
            this->_arc->_pool = nullptr;
        });
    }

    // arc

    // Destructors

    connection :: arc :: ~arc()
    {
        this->_socket.match([](auto & socket)
        {
            if constexpr (!std :: is_const <std :: remove_reference_t <decltype(socket)>> :: value)
                socket.close();
        });
    }

    // channelpair

    // Constructors

    connection :: arc :: channelpair :: channelpair(const class channel :: key & receivekey, const class channel :: nonce & receivenonce, const class channel :: key & transmitkey, const class channel :: nonce & transmitnonce) : receive(receivekey, receivenonce), transmit(transmitkey, transmitnonce)
    {
    }
};
