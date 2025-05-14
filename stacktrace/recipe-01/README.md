# 在 Linux C/C++ 程序中打印调用栈

在 Linux 环境下，有几种方法可以在 C/C++ 程序中打印当前调用栈（stack trace）。以下是几种常见的方法和示例：

## 1. 使用 glibc 的 backtrace 函数

这是最常用的方法，需要包含 `<execinfo.h>` 头文件。

```c
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_stacktrace() {
    void *array[10];
    size_t size;
    char **strings;
    size_t i;

    size = backtrace(array, 10);
    strings = backtrace_symbols(array, size);

    printf("Obtained %zd stack frames.\n", size);

    for (i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }

    free(strings);
}

void foo() {
    print_stacktrace();
}

void bar() {
    foo();
}

int main() {
    bar();
    return 0;
}
```

编译时需要加上 `-rdynamic` 选项：
```bash
gcc -rdynamic program.c -o program
```

## 2. 使用 libunwind 库

libunwind 提供了更强大的堆栈展开功能：

```c
#include <libunwind.h>
#include <stdio.h>

void print_stacktrace() {
    unw_cursor_t cursor;
    unw_context_t context;

    // Initialize cursor to current frame
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // Unwind frames one by one, going up the stack
    while (unw_step(&cursor) > 0) {
        unw_word_t offset, pc;
        char sym[256];

        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if (pc == 0) {
            break;
        }

        printf("0x%lx: ", (long)pc);

        if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
            printf("(%s+0x%lx)\n", sym, (long)offset);
        } else {
            printf("-- no symbol name found\n");
        }
    }
}

void foo() {
    print_stacktrace();
}

int main() {
    foo();
    return 0;
}
```

编译时需要链接 libunwind：
```bash
gcc program.c -o program -lunwind -lunwind-x86_64
```

## 3. 使用 Boost 库（C++）

如果你使用 C++ 并且有 Boost 可用：

```cpp
#include <boost/stacktrace.hpp>
#include <iostream>

void foo() {
    std::cout << boost::stacktrace::stacktrace();
}

int main() {
    foo();
    return 0;
}
```

编译时需要链接 Boost.Stacktrace：
```bash
g++ program.cpp -o program -lboost_stacktrace_backtrace -ldl
```

## 注意事项

1. 在生产环境中打印调用栈可能会影响性能
2. 某些情况下（如信号处理程序中），可能需要特殊的处理方式
3. 要获得有意义的符号名称，可能需要：
   - 编译时加上 `-g` 选项保留调试信息
   - 使用 `addr2line` 或 `dladdr` 等工具将地址转换为函数名和行号
4. 在信号处理程序中使用时，backtrace 函数应该是异步信号安全的

以上方法都可以帮助你在程序运行时获取调用栈信息，便于调试和错误诊断。
