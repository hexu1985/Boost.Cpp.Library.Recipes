#include <boost/stacktrace.hpp>
#include <iostream>

void foo() {
    std::cout << boost::stacktrace::stacktrace();
}

void bar() {
    foo();
}

int main() {
    bar();
    return 0;
}
