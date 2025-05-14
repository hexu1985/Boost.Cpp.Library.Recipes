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

void bar() {
    foo();
}

int main() {
    bar();
    return 0;
}
