#pragma once

#include <string>
#include <iostream>
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>

class Person {
private:
    friend class boost::serialization::access;

    // 序列化函数（支持版本控制）
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & name;
        ar & age;
    }

    std::string name;
    int age;

public:
    Person() = default;
    Person(const std::string& name, int age) : name(name), age(age) {}

    // 打印信息
    void print() const {
        std::cout << "Name: " << name << ", Age: " << age << std::endl;
    }
};
