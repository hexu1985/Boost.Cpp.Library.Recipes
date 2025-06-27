#pragma once

#include <iostream>
#include <atomic>

template< typename DerivedT >
class intrusive_ref_counter;

template< typename DerivedT >
void intrusive_ptr_add_ref(const intrusive_ref_counter< DerivedT >* p) noexcept;
template< typename DerivedT >
void intrusive_ptr_release(const intrusive_ref_counter< DerivedT >* p) noexcept;

template< typename DerivedT >
class intrusive_ref_counter
{
private:
    //! Reference counter
    mutable std::atomic_int m_ref_counter;

public:
    /*!
     * Default constructor
     *
     * \post <tt>use_count() == 0</tt>
     */
    intrusive_ref_counter() noexcept : m_ref_counter(0)
    {
        std::cout << "Reference Counter Constructor" << std::endl;
    }

    /*!
     * Copy constructor
     *
     * \post <tt>use_count() == 0</tt>
     */
    intrusive_ref_counter(intrusive_ref_counter const&) noexcept : m_ref_counter(0)
    {
        std::cout << "Reference Counter Copy Constructor" << std::endl;
    }

    /*!
     * Assignment
     *
     * \post The reference counter is not modified after assignment
     */
    intrusive_ref_counter& operator= (intrusive_ref_counter const&) noexcept 
    {
        std::cout << "Reference Counter Assignment Operator" << std::endl;
        return *this; 
    }

    /*!
     * \return The reference counter
     */
    unsigned int use_count() const noexcept
    {
        return m_ref_counter;
    }

private:
    void add_ref() const
    {
        ++m_ref_counter;
    }

    void release() const
    {
        if (--m_ref_counter == 0) {
            delete this;
        }
    }

protected:
    /*!
     * Destructor
     */
    ~intrusive_ref_counter() 
    {
        std::cout << "Reference Counter Destructor" << std::endl;
    }

    friend void intrusive_ptr_add_ref< DerivedT >(const intrusive_ref_counter< DerivedT >* p) noexcept;
    friend void intrusive_ptr_release< DerivedT >(const intrusive_ref_counter< DerivedT >* p) noexcept;
};

template< typename DerivedT >
inline void intrusive_ptr_add_ref(const intrusive_ref_counter< DerivedT >* p) noexcept
{
    p->add_ref();
}

template< typename DerivedT >
inline void intrusive_ptr_release(const intrusive_ref_counter< DerivedT >* p) noexcept
{
    p->release();
}
