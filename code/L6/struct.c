#include <stdalign.h>
#include <stdio.h>
#include <string.h>

struct Position {
    double x;
    double y;
    double z;
};

typedef struct {
    double x;
    double y;
    double z;
} Pos;

void print_position(Pos *pos) {
    printf("x=%f; y=%f; z=%f\n", pos->x, pos->y, pos->z);
}

typedef struct {
    char a; // 1B
    int b;  // 4B
    char c; // 1B
} Bar;

typedef struct __attribute__((packed)) {
    char a; // 1B
    int b;  // 4B
    char c; // 1B
} BarPacked;

int main() {
    // double x, y, z;
    Pos pos = {.x = 0.1, .y = 0.2, .z = 0.3};
    print_position(&pos);

    Pos pos2 = pos;
    pos.x = 1.0;
    print_position(&pos2);

    printf("&x=%p; &y=%p; &z=%p\n", &pos.x, &pos.y, &pos.z);

    printf("sizeof(Bar) is %zu\n", sizeof(Bar));
    printf("alignof(char)=%zu; alignof(int)=%zu;\n", alignof(char), alignof(int));

    Bar bar;
    printf("&a: %p; &b: %p; &c: %p\n", &bar.a, &bar.b, &bar.c);

    printf("alignof(Bar): %zu\n", alignof(Bar));

    printf("sizeof(BarPacked): %zu\n", sizeof(BarPacked));

    BarPacked bp = { 0 };
    int x;
    memcpy(&x, &bp.b, sizeof(x));
    printf("x=%d\n", x);
}
