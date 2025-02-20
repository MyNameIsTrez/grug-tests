#include <stdio.h>

int main() {
    foo();
    printf("a\n");
    printf("b\n");
}

int foo() {
    printf("c\n");
    foo();
}

int bar() {
    printf("c\n");
    foo();
}
