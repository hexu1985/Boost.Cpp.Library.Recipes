#include "ProcessData.h"
#include "intrusive_ptr.hpp"

int main()
{
    intrusive_ptr<ProcessData> ptr(new ProcessData(1, "a"));
    std::cout << "******************"<< std::endl;
    std::cout << "ref_count = " << ptr->RefCount() << std::endl;
    std::cout << "******************" << std::endl;
    {
        intrusive_ptr<ProcessData> ptrCopy(ptr.get());
        std::cout << "ref_count after copy constructed = " << ptrCopy->RefCount() << std::endl;
    }

    std::cout << "******************" << std::endl;
    std::cout << "ref_count = " << ptr->RefCount() << std::endl;
    std::cout << "******************" << std::endl;

    {
        intrusive_ptr<ProcessData> ptrAssignment = ptr;
        std::cout << "ref_count after assignment = " << ptrAssignment->RefCount() << std::endl;
    }

    std::cout << "******************" << std::endl;
    std::cout << "ref_count = " << ptr->RefCount() << std::endl;
    std::cout << "******************" << std::endl;

    {
        intrusive_ptr<ProcessData> ptrWeak(ptr.get(), false);
        std::cout << "ref_count after construct weak_ptr = " << ptrWeak->RefCount() << std::endl;
        ptrWeak.detach();
    }

    std::cout << "******************" << std::endl;
    std::cout << "ref_count = " << ptr->RefCount() << std::endl;
    std::cout << "******************" << std::endl;
    
    return 0;
}

