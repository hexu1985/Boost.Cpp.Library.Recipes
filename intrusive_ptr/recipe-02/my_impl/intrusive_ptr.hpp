#pragma once

#include <cassert>

template <typename T>
class intrusive_ptr {
private:
    typedef intrusive_ptr this_type;

public:
    constexpr intrusive_ptr() noexcept : px( 0 )
    {
    }

    intrusive_ptr( T * p, bool add_ref = true ): px( p )
    {
        if( px != 0 && add_ref ) intrusive_ptr_add_ref( px );
    }

    intrusive_ptr(intrusive_ptr const & rhs): px( rhs.px )
    {
        if( px != 0 ) intrusive_ptr_add_ref( px );
    }

    ~intrusive_ptr()
    {
        if( px != 0 ) intrusive_ptr_release( px );
    }

    intrusive_ptr & operator=(intrusive_ptr const & rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    intrusive_ptr & operator=(T * rhs)
    {
        this_type(rhs).swap(*this);
        return *this;
    }

    void reset()
    {
        this_type().swap( *this );
    }

    void reset( T * rhs )
    {
        this_type( rhs ).swap( *this );
    }

    void reset( T * rhs, bool add_ref )
    {
        this_type( rhs, add_ref ).swap( *this );
    }

    T * get() const noexcept
    {
        return px;
    }

    T * detach() noexcept
    {
        T * ret = px;
        px = 0;
        return ret;
    }

    T & operator*() const
    {
        assert( px != 0 );
        return *px;
    }

    T * operator->() const
    {
        assert( px != 0 );
        return px;
    }

    explicit operator bool () const noexcept
    {
        return px != 0;
    }

    void swap(intrusive_ptr & rhs) noexcept
    {
        T * tmp = px;
        px = rhs.px;
        rhs.px = tmp;
    }

private:
    T * px;
};
