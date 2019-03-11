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
