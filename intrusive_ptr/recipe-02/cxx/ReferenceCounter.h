#ifndef REFERENCE_COUNTER_H
#define REFERENCE_COUNTER_H

#include <assert.h>
#include <atomic>
#include <iostream>

class ReferenceCounter
{
public:
    ReferenceCounter() :ref_count(0)
    {
        std::cout << "Reference Counter Constructor" << std::endl;
    }

    ReferenceCounter(const ReferenceCounter &other): ref_count(0)
    {
        std::cout << "Reference Counter Copy Constructor" << std::endl;
    }

    ReferenceCounter& operator=(const ReferenceCounter &other)
    {
        std::cout << "Reference Counter Assignment Operator" << std::endl;
        return *this;
    }
    
    ~ReferenceCounter() 
    {
        std::cout << "Reference Counter Destructor" << std::endl;
    };

    int RefCount()
    {
        return ref_count;
    }

    void AddRef()
    {
        ++ref_count;
    }

    void ReleaseRef()
    {
        if (--ref_count == 0)
        {
            delete this;
        }
    }

private:
    std::atomic_int ref_count;
};

static inline void intrusive_ptr_add_ref(ReferenceCounter *p)
{
    std::cout << "Call intrusive_ptr_add_ref" << std::endl;
    assert(p);
    assert(p->RefCount() >= 0);
    p->AddRef();
}

static inline void intrusive_ptr_release(ReferenceCounter *p)
{
    std::cout << "Call intrusive_ptr_release" << std::endl;
    assert(p);
    assert(p->RefCount() > 0);
    p->ReleaseRef();
}

#endif
