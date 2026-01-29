#include <stdio.h>
#include <stdlib.h>

int global = 528491, global_uninit;

void bar() {
    int x = 10;
    printf("Addr of x in bar: %p\n", &x);
}

void foo() {
    int x = 0;
    x += 1;
    // printf("%d\n", x);
    printf("Addr of x in foo: %p\n", &x);
    bar();

    static int y = 0;
    y += 1;
    printf("y is %d\n", y); // y's value is preserved across function invocations
}

void huge_stack_frame() {
    int arr[1000000]; // The code will fail if we have 10M elements
    arr[1] = 0;
    printf("arr[1] = %d\n", arr[1]);
}

void recursion(int cnt) {
    if (cnt == 0) return; // The code will fail if we have a large cnt
    recursion(cnt - 1);
}

int *AVOID_THIS(int x) {
    int local = x + 1;
    return &local; // Don't do this
}

void uninit() {
    int z;
    printf("addr of z: %p; val of z: %d\n", &z, z);
}

void override(int val) {
    int x = val;
    printf("overriding %p with %d\n", &x, val);
}

int main() {
    foo();
    foo();
    printf("Addr of global: %p\n", &global);
    printf("Addr of global_uninit: %p\n", &global_uninit);

    huge_stack_frame();
    // recursion(1000000); // This will trigger segfault

    uninit();
    override(10);
    uninit();
    override(20);
    uninit();

    int *x = malloc(sizeof(int));
    if (x) {
        *x = 10;
        printf("x point to %p, val=%d\n", x, *x);
        free(x);
    }

    // allocating an 1-d array
    // int *dyn_arr = malloc(sizeof(int) * 100);
    // memset(dyn_arr, 0, sizeof(int) * 100);
    int n = 100;
    int *dyn_arr = calloc(n, sizeof(int));
    if (dyn_arr) {
        dyn_arr[10] = 12345;
        printf("dyn_arr lives at %p; dyn_arr[10]=%d\n", dyn_arr, dyn_arr[10]);
        free(dyn_arr);
    }

    // allocating a 2-d array
    double **dyn_2d_arr = calloc(3, sizeof(double *));
    for (int i = 0; i < 3; i++) {
        dyn_2d_arr[i] = calloc(3, sizeof(double));
    }

    dyn_2d_arr[2][2] = 3.14;
    printf("dyn_2d_arr[2][2]=%f\n", dyn_2d_arr[2][2]);

    for (int i = 0; i < 3; i++) {
        free(dyn_2d_arr[i]);
    }
    free(dyn_2d_arr);
}
