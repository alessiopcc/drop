// Includes

#include "tcp.hpp"
#include "pool.hpp"
#include "connection.hpp"
#include "drop/exception/exception.hpp"
#include "drop/async/promise.hpp"

namespace drop
{
    // tcp

    // Static methods

    connection tcp :: connectsync(const address & remote)
    {
        socket socket = remote.is <IPv4> () ? socket :: IPv4() : socket :: IPv6();
        socket.connect(remote);
        return connection(socket);
    }

    promise <connection> tcp :: connectasync(const address & remote)
    {
        return connectasync(remote, pool :: system.get());
    }

    promise <connection> tcp :: connectasync(const address & remote, pool & pool)
    {
        socket socket = remote.is <IPv4> () ? socket :: IPv4() : socket :: IPv6();
        socket.set <blocking> (false);
        socket.connect(remote);

        co_await pool.write(socket);
        co_return connection(socket);
    }

    promise <connection> tcp :: connect(const address & remote)
    {
        return connectasync(remote);
    }

    promise <connection> tcp :: connect(const address & remote, pool & pool)
    {
        return connectasync(remote, pool);
    }

    drop :: listener tcp :: listen(const class address :: port & port)
    {
        return drop :: listener :: construct <listener> (port);
    }

    drop :: listener tcp :: listen(const address & address)
    {
        return drop :: listener :: construct <listener> (address);
    }

    // socket

    // Private constructors

    tcp :: socket :: socket(const int & descriptor) : _descriptor(descriptor)
    {
    }

    // Getters

    address tcp :: socket :: remote() const
    {
        sockaddr_storage remote;
        socklen_t socklen = sizeof(sockaddr_storage);

        if(getpeername(this->_descriptor, (struct sockaddr *) &remote, &socklen))
            exception <bad_access, getpeername_failed> :: raise(this, errno);

        if(((struct sockaddr *) &remote)->sa_family == AF_INET)
            return address(*((struct sockaddr_in *) &remote));
        else
            return address(*((struct sockaddr_in6 *) &remote)).decay();
    }

    // Methods

    void tcp :: socket :: bind(const class address :: port & port)
    {
        if(this->_type == PF_INET)
            this->bind(address(address :: ip :: any <drop :: IPv4> (), port));
        else
            this->bind(address(address :: ip :: any <drop :: IPv6> (), port));
    }

    void tcp :: socket :: bind(const address & address)
    {
        this->opencheck();

        address.match([&](const sockaddr_in & sockaddr)
        {
            if(:: bind(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in)))
                exception <bind_failed> :: raise(this, errno);
        }, [&](const sockaddr_in6 & sockaddr)
        {
            if(:: bind(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in6)))
                exception <bind_failed> :: raise(this, errno);
        });
    }

    void tcp :: socket :: listen() const
    {
        this->opencheck();

        if(:: listen(this->_descriptor, settings :: listen :: slots))
            exception <listen_failed> :: raise(this, errno);
    }

    tcp :: socket tcp :: socket :: accept() const
    {
        this->opencheck();

        sockaddr_storage remote;
        socklen_t socklen = sizeof(sockaddr_storage);

        int descriptor = :: accept(this->_descriptor, (struct sockaddr *) &remote, &socklen);

        if(descriptor < 0 && (errno != EAGAIN) && (errno != EWOULDBLOCK))
            exception <accept_failed> :: raise(this, errno);

        return socket(descriptor);
    }

    void tcp :: socket :: connect(const address & remote)
    {
        this->opencheck();

        int status;

        remote.match([&](const sockaddr_in & sockaddr)
        {
            status = :: connect(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in));
        }, [&](const sockaddr_in6 & sockaddr)
        {
            status = :: connect(this->_descriptor, (const struct sockaddr *) &sockaddr, sizeof(sockaddr_in6));
        });

        if(status && (errno != EINPROGRESS))
            exception <connect_failed> :: raise(this, errno);
    }

    size_t tcp :: socket :: send(const uint8_t * message, const size_t & size) const
    {
        this->opencheck();

        ssize_t status = :: send(this->_descriptor, message, size, 0);

        if(status < 0)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
                return 0;

            exception <send_failed> :: raise(this, errno);
        }

        return (size_t) status;
    }

    size_t tcp :: socket :: receive(uint8_t * message, const size_t & size) const
    {
        this->opencheck();

        ssize_t status = :: recv(this->_descriptor, message, size, 0);

        if(status < 0)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
                return 0;

            exception <receive_failed> :: raise(this, errno);
        }

        return (size_t) status;
    }

    void tcp :: socket :: close()
    {
        :: close(this->_descriptor);
        this->_descriptor = -1;
    }

    // Private methods

    void tcp :: socket :: opencheck() const
    {
        if(this->_descriptor < 0)
            exception <bad_access, socket_closed> :: raise(this);
    }

    int tcp :: socket :: fcntl() const
    {
        this->opencheck();

        int flags = :: fcntl(this->_descriptor, F_GETFL);

        if(flags < 0)
            exception <bad_access, fcntl_failed> :: raise(this, errno);

        return flags;
    }

    void tcp :: socket :: fcntl(const int & flags) const
    {
        this->opencheck();

        if(:: fcntl(this->_descriptor, F_SETFL, flags))
            exception <bad_access, fcntl_failed> :: raise(this, errno);
    }

    int tcp :: socket :: ioctl(const unsigned long & request) const
    {
        this->opencheck();

        int value;

        if(:: ioctl(this->_descriptor, request, &value))
            exception <ioctl_failed> :: raise(this, errno);

        return value;
    }

    // Casting

    tcp :: socket :: operator bool () const
    {
        return (this->_descriptor >= 0);
    }

    // Static methods

    tcp :: socket tcp :: socket :: IPv4()
    {
        socket socket(:: socket(PF_INET, SOCK_STREAM, 0));
        socket._type = PF_INET;
        socket.set <reuse> (true);
        return socket;
    }

    tcp :: socket tcp :: socket :: IPv6()
    {
        socket socket(:: socket(PF_INET6, SOCK_STREAM, 0));
        socket._type = PF_INET6;
        socket.setsockopt(IPPROTO_IPV6, IPV6_V6ONLY, 1);
        socket.set <reuse> (true);
        return socket;
    }

    tcp :: socket tcp :: socket :: any()
    {
        socket socket(:: socket(PF_INET6, SOCK_STREAM, 0));
        socket._type = PF_INET6;
        socket.setsockopt(IPPROTO_IPV6, IPV6_V6ONLY, 0);
        socket.set <reuse> (true);
        return socket;
    }

    // listener

    // Constructors

    tcp :: listener :: listener(const class address :: port & port) : _socket(socket :: IPv6()), _cache{.blocking = false}, _lock(false), _pool(nullptr)
    {
        this->_socket.bind(port);
        this->_socket.listen();
    }

    tcp :: listener :: listener(const address & address) : _socket(address.is <IPv4> () ? socket :: IPv4() : socket :: IPv6()), _cache{.blocking = false}, _lock(false), _pool(nullptr)
    {
        this->_socket.bind(address);
        this->_socket.listen();
    }

    // Destructor

    tcp :: listener :: ~listener()
    {
        this->_socket.close();
    }

    // Methods

    connection tcp :: listener :: acceptsync()
    {
        this->setup <true> ();

        try
        {
            connection connection(this->_socket.accept());
            this->release();
            return connection;
        }
        catch(...)
        {
            this->release();
            std :: rethrow_exception(std :: current_exception());
        }
    }

    promise <connection> tcp :: listener :: acceptasync()
    {
        this->setup <false> ();

        try
        {
            pool * binding = this->_guard([&](){return this->_pool;});

            co_await (binding ? *binding : pool :: system.get()).read(this->_socket);
            connection connection(this->_socket.accept());

            this->release();
            co_return connection;
        }
        catch(...)
        {
            this->release();
            std :: rethrow_exception(std :: current_exception());
        }
    }

    void tcp :: listener :: bind(pool & pool)
    {
        this->_guard([&]()
        {
            this->_pool = &pool;
        });
    }

    void tcp :: listener :: unbind()
    {
        this->_guard([&]()
        {
            this->_pool = nullptr;
        });
    }

    // Private methods

    void tcp :: listener :: release()
    {
        this->_guard([&]()
        {
            this->_lock = false;
        });
    }
};
