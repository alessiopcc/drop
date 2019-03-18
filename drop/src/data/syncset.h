// Forward declarations

namespace drop
{
    template <typename> class syncset;
};

#if !defined(__forward__) && !defined(__src__data__syncset__h)
#define __src__data__syncset__h

// Includes

#include "variant.hpp"
#include "varint.h"
#include "drop/bytewise/bytewise.hpp"
#include "drop/crypto/hash.hpp"

namespace drop
{
    template <typename type> class syncset
    {
        // Asserts

        static_assert(std :: is_copy_constructible <type> :: value, "Syncset type must be copy constructible.");
        static_assert(bytewise :: constraints :: readable <type, hash :: state> (), "Syncset type must be hashable.");
        static_assert((bytewise :: constraints :: serializable <type> () && bytewise :: constraints :: deserializable <type> ()), "Syncset type must be serializable / deserializable.");

        // Settings

        struct settings
        {
            struct thresholds
            {
                static constexpr size_t list = 8;
            };
        };

    public: // REMOVE ME

        // Service nested enums

        enum navigation {left, right};

        // Service nested classes

        class path;
        class prefix;
        class navigator;

        class empty;
        class single;
        class multiple;

        typedef variant <empty, single, multiple> node;

        // Nested classes

        class labelset;
        class listset;

        typedef variant <labelset, listset> set;
        typedef std :: vector <set> view;

        // Nested structs

        struct round;

    private:

        // Members

        node * _root;

    public:

        // Constructors

        syncset();
        syncset(const syncset &);
        syncset(syncset &&);

        // Destructor

        ~syncset();

        // Methods

        void add(const type &);
        void remove(const type &);

        bool find(const type &);

        round sync();
        round sync(const view &);

    private:

        // Private methods

        template <bool dump> set get(const prefix &);

    public:

        // Operators

        syncset & operator = (const syncset &);
        syncset & operator = (syncset &&);
    };

    template <typename type> class syncset <type> :: path : public hash
    {
    public:

        // Constructors

        path(const hash &);
        path(const type &);

        // Operators

        bool operator [] (const size_t &) const;
    };

    template <typename type> class syncset <type> :: prefix : public path
    {
        // Members

        varint _depth;

    public:

        // Constructors

        prefix(const path &, const size_t &);

        // Bytewise

        template <typename vtype> void accept(bytewise :: reader <vtype> &) const;
        template <typename vtype> void accept(bytewise :: writer <vtype> &);

        // Getters

        const size_t & depth() const;

        // Methods

        prefix left() const;
        prefix right() const;
    };

    template <typename type> class syncset <type> :: navigator
    {
        // Members

        path _path;
        size_t _depth;

        std :: array <node *, 8 * hash :: size> _trace;

    public:

        // Constructors

        navigator(const path &, node &);

        // Getters

        const size_t & depth() const;
        navigation next() const;

        // Operators

        node & operator * ();
        node * operator -> ();

        navigator & operator ++ ();
        navigator & operator ++ (int);

        navigator & operator -- ();
        navigator & operator -- (int);

        // Casting

        operator bool () const;
    };

    template <typename type> class syncset <type> :: empty
    {
        // Static members

        static hash _label;

    public:

        // Getters

        const hash & label() const;

        // Methods

        single fill(const type &);
        template <typename lambda> void traverse (lambda &&) const;
    };

    template <typename type> class syncset <type> :: single
    {
        // Members

        hash _label;
        type _element;

    public:

        // Constructors

        single(const type &);

        // Getters

        const hash & label() const;
        const type & element() const;

        // Methods

        empty empty();
        multiple push(const navigation &);

        template <typename lambda> void traverse(lambda &&) const;
    };

    template <typename type> class syncset <type> :: multiple
    {
        // Members

        hash _label;
        size_t _size;

        node * _left;
        node * _right;

    public:

        // Constructors

        multiple(const node &, const node &);
        multiple(const multiple &);
        multiple(multiple &&);

        // Destructor

        ~multiple();

        // Getters

        const hash & label() const;
        const size_t & size() const;

        node * left() const;
        node * right() const;

        // Methods

        void refresh();

        bool pullable();
        single pull();

        template <typename lambda> void traverse(lambda &&) const;

        // Operators

        multiple & operator = (const multiple &);
        multiple & operator = (multiple &&);
    };

    template <typename type> class syncset <type> :: labelset
    {
        // Memberss

        prefix _prefix;
        hash _label;

        // Bytewise

        $bytewise(_prefix);
        $bytewise(_label);

    public:

        // Constructors

        labelset(const prefix &, const multiple &);

        // Getters

        const prefix & prefix() const;
        const hash & label() const;

        // Operators

        bool operator == (const labelset &) const;
        bool operator != (const labelset &) const;
    };

    template <typename type> class syncset <type> :: listset : std :: vector <type>
    {
        // Members

        prefix _prefix;
        bool _dump;

        // Bytewise

        $bytewise(_prefix);
        $bytewise(base, (std :: vector <type>));
        $bytewise(_dump);

    public:

        // Constructors

        listset(const prefix &, const empty &, const bool &);
        listset(const prefix &, const single &, const bool &);
        listset(const prefix &, const multiple &, const bool &);

        // Getters

        const prefix & prefix() const;
        const bool & dump() const;

        // Operators

        bool operator == (const listset &) const;
        bool operator != (const listset &) const;
    };

    template <typename type> struct syncset <type> :: round
    {
        view view;
        std :: vector <type> add;
        std :: vector <type> remove;
    };
};

#endif
