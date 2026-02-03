#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_int_array(int *arr, size_t sz) {
    printf("arr: %p;", arr);
    for (size_t i = 0; i < sz; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
}

void common_bugs() {
    const size_t sz = 10;

    // buffer overflow
    char *str = calloc(sz, sizeof(char));
    // strcpy(str, "Very very very long string"); // This would cause a buffer overflow
    strncpy(str, "Very very very long string", sz - 1);

    // buffer underflow
    str[-1] = '\0'; // BAD!!!

    // out-of-bound read
    int idx;
    scanf("%d", &idx);
    if (idx < sz && idx >= 0) { // always add bounds check
        printf("reading %d\n", str[idx]);
    }

    // use-after-free
    int *old = malloc(sizeof(int));
    *old = 10;
    free(old);

    int *new = malloc(sizeof(int));
    *old = 20; // use after free. BAD!!!
    printf("old: %p; new: %p; *new: %d\n", old, new, *new);

    // double free
    int *a = malloc(sizeof(int));
    int *b = malloc(sizeof(int));

    free(a);
    free(b);
    free(a); // double free. BAD!!!

    int *c = malloc(sizeof(int));
    int *d = malloc(sizeof(int));
    int *e = malloc(sizeof(int));

    *c = 10;
    *e = 20;
    printf("a: %p; b: %p; c: %p; d: %p; e: %p\n", a, b, c, d, e);
    printf("*c: %d\n", *c);

    // free a variable not allocated by malloc
    // int x;
    // free(&x);

    // free(str + 1); // Must free a base pointer
}

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    size_t sz = strtoul(argv[1], NULL, 10);
    if (sz < 10) return 1;

    int *array = calloc(sz, sizeof(*array));
    if (!array) return 1;

    for (size_t i = 0; i < sz; i++) {
        array[i] = i;
    }

    print_int_array(array, sz);

    const size_t scale = 5;
    sz = sz * scale;
    array = realloc(array, sizeof(int) * sz);
    print_int_array(array, sz);

    free(array);

    const size_t ncols = 4, nrows = 3;
    // double *row0 = calloc(ncols, sizeof(double));
    // double *row1 = calloc(ncols, sizeof(double));
    // double *row2 = calloc(ncols, sizeof(double));

    // double *rows[] = {row0, row1, row2};
    // rows[1][2] = 0.1234;

    double **matrix = calloc(nrows, sizeof(double *));
    for (size_t r = 0; r < nrows; r++) {
        matrix[r] = calloc(ncols, sizeof(double));
    }

    matrix[1][2] = 0.2345;

    for (size_t r = 0; r < nrows; r++) {
        free(matrix[r]);
    }
    free(matrix);

    common_bugs();
}
