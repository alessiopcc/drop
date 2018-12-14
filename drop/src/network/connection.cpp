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
    }

    // arc

    // Destructors

    connection :: arc :: ~arc()
    {
        this->_socket.match([](auto & socket)
        {
            if constexpr (!std :: is_const <std :: remove_reference_t <decltype(socket)>> :: value)
            {
                socket.close();
            }
        });
    }
};
