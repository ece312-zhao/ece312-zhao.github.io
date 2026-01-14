#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int solve_quadratic(double a, double b, double c, double *x1, double *x2) {
    double dis = b * b - 4 * a * c;

    if (dis < 0) {
        return 0;
    } else if (dis == 0) {
        *x1 = -b / a / 2;
        return 1;
    } else {
        double tmp = sqrt(dis);

        *x1 = (-b + tmp) / a / 2;
        *x2 = (-b - tmp) / a / 2;
        return 2;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Too few arguments\n");
        return 1;
    }

    double a = strtod(argv[1], NULL), b = strtod(argv[2], NULL),
           c = strtod(argv[3], NULL);

    if (a == 0) {
        printf("a must be non-zero!\n");
        return 2;
    }

    double x1, x2;
    int nsol = solve_quadratic(a, b, c, &x1, &x2);
    printf("# solutions=%d; Solutions: x1=%f; x2=%f\n", nsol, x1, x2);

    return 0;
}

