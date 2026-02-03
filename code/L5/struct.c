#include <stdio.h>

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

int main() {
    // double x, y, z;
    Pos pos = {.x = 0.1, .y = 0.2, .z = 0.3};
    print_position(&pos);
}
