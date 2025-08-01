#include "Person.hpp"
#include <fstream>
#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

int main() {
    // 1. 序列化到文件
    {
        Person person("Alice", 30);
        std::ofstream ofs("person.dat");
        boost::archive::text_oarchive oa(ofs);
        oa << person; // 序列化
        std::cout << "Serialized: ";
        person.print();
    }

    // 2. 从文件反序列化
    {
        Person new_person;
        std::ifstream ifs("person.dat");
        boost::archive::text_iarchive ia(ifs);
        ia >> new_person; // 反序列化
        std::cout << "Deserialized: ";
        new_person.print();
    }

    return 0;
}
