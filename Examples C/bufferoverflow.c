#include <stdio.h>
#include <string.h>

void safe() {
    printf("Returned safely\n");
}

void secret() {
    printf("Control hijacked! Jumped to secret function\n");
}

struct Data {
    char buffer[8];
    void (*func_ptr)();
};

void vulnerable() {
    struct Data d;

    d.func_ptr = safe;

    printf("Enter input: ");
    gets(d.buffer);   // unsafe

    // DEBUG: show what happened
    printf("Buffer length: %lu\n", strlen(d.buffer));

    // Force visible effect when overflow happens
    if (strlen(d.buffer) > 8) {
        printf("Overflow condition triggered!\n");
        d.func_ptr = secret;
    }

    d.func_ptr();
}

int main() {
    vulnerable();
    return 0;
}
