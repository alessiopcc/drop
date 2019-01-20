// Includes

#include "listener.hpp"

namespace drop
{
    // Constructors

    listener :: listener(const std :: shared_ptr <variant> & arc) : _arc(arc)
    {
    }

    // Methods

    connection listener :: acceptsync() const
    {
        optional <connection> connection;

        this->_arc->match([&](auto & listener)
        {
            if constexpr (!std :: is_const <std :: remove_reference_t <decltype(listener)>> :: value)
                connection = listener.acceptsync();
        });

        return *connection;
    }

    promise <connection> listener :: acceptasync() const
    {
        promise <connection> promise;

        this->_arc->match([&](auto & listener)
        {
            if constexpr (!std :: is_const <std :: remove_reference_t <decltype(listener)>> :: value)
                promise = listener.acceptasync();
        });

        return promise;
    }

    promise <connection> listener :: accept() const
    {
        return this->acceptasync();
    }

    void listener :: bind(pool & pool) const
    {
        this->_arc->match([&](auto & listener)
        {
            if constexpr (!std :: is_const <std :: remove_reference_t <decltype(listener)>> :: value)
                listener.bind(pool);
        });
    }

    void listener :: unbind() const
    {
        this->_arc->match([&](auto & listener)
        {
            if constexpr (!std :: is_const <std :: remove_reference_t <decltype(listener)>> :: value)
                listener.unbind();
        });
    }
};
