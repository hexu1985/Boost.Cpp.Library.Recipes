// backtrace_unwind.cpp
#include <iostream>
#include <cstdio>
#include <libunwind.h>
#include <cxxabi.h> // 用于 demangle C++ 符号名

void print_backtrace() {
    unw_cursor_t cursor;
    unw_context_t context;

    // 初始化游标到当前上下文
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // 打印堆栈跟踪的每一帧
    std::cout << "Backtrace:" << std::endl;

    int frame_num = 0;
    while (unw_step(&cursor) > 0) {
        unw_word_t offset, pc;
        char sym[256];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if (pc == 0) {
            break;
        }

        char *name = sym;
        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
            // Demangle C++ 符号名
            int status;
            char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
            if (demangled) {
                name = demangled;
            }

            std::printf("#%-2d 0x%016lx %s + 0x%lx\n", 
                       frame_num++, pc, name, offset);
            
            if (demangled) {
                free(demangled);
            }
        } else {
            std::printf("#%-2d 0x%016lx -- unknown symbol --\n", 
                       frame_num++, pc);
        }
    }
}

void foo() {
    print_backtrace();
}

void bar() {
    foo();
}

int main() {
    bar();
    return 0;
}
