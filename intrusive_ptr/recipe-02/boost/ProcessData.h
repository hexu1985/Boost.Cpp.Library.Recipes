#ifndef PROCESS_DATA_H
#define PROCESS_DATA_H

#include <string>
#include <iostream>

#include <boost/smart_ptr/intrusive_ref_counter.hpp>

class ProcessData : public boost::intrusive_ref_counter<ProcessData>
{
public:
    ProcessData(int id, std::string info) :m_id(id), m_info(info) 
    {
        std::cout << "Process Data Constructor" << std::endl;
    }

    ProcessData(const ProcessData &other)
    {
        std::cout << "Process Data Copy Constructor" << std::endl;
        m_id = other.m_id;
        m_info = other.m_info;
    }

    const ProcessData& operator=(const ProcessData &other)
    {
        std::cout << "Process Data Assignment Operator" << std::endl;
        m_id = other.m_id;
        m_info = other.m_info;
        return *this;
    }

    ~ProcessData()
    {
        std::cout << "Process Data Destructor" << std::endl;
    }

    int RefCount()
    {
        return use_count();
    }

private:
    int m_id;
    std::string m_info;
};

#endif 
