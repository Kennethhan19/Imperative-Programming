#include <stdio.h>

int square(int x) {
    int a = x*x;
    return a;
}

int main() {
    int n = square(50000);
    printf("square of 42 is %d\n", n);
    return 0;
}

// #include <stdio.h>
//
// int square(int n) {
//     return n * n;
// }
//
// int main() {
//     int n = square(42);
//     printf("42 squared is %d\n", n);
//     return 0;
// }
