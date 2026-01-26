#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int global = 1;

void print_int(int x) {
    printf("Int is %d\n", x);
}

int compare_int(const void *a, const void *b) {
    // void * represents a pointer with any type (except for a function pointer)
    const int *A = a; // implicit conversion from const void * to const int *
    const int *B = b;
    return *A - *B;
}

int main() {
    int local = 10;
    int *ptr = &local; // Defining a pointer that point to local

    // Use format specifier %p
    printf("ptr=%p; the value=%d\n", ptr, *ptr);

    // Pointers to different varibales
    { // a different code block
        int local = 20; // 'local' shadows the 'local' from the outside
        printf("Pointer to local in the block: %p\n", &local);
    }
    printf("Pointer to local in the function: %p\n", &local);
    printf("Pointer to global: %p\n", &global);

    // Pointer arithmetic: increment at the size of the data type to which it points
    int *new_ptr = ptr + 1;
    printf("Ptr: %p; New ptr: %p\n", ptr, new_ptr); // new_ptr - ptr = sizeof(int)

    // Use a pointer to a pointer
    char *str = "123 abcd";
    char *end;
    // strtod sets `end` such that it points to the first unparsed character
    // this is achieved by passing a pointer to end (i.e., a pointer to pointer)
    double res = strtod(str, &end);
    printf("str: %p; end: %p; end str: %s\n", str, end, end);
    if (str == end) {
        // If end didn't move at all, it means nothing is parsed
        printf("Invalid user input: \"%s\"\n", end);
    }

    char *p_null = NULL; // NULL pointer means that the pointer is invalid
    // printf("Deference p_null: %c", *p_null); // This will trigger segmentation fault

    // Array and pointers
    int arr[] = {1, 2, 3, 4, 5};
    int *intp;

    // Array decaying
    intp = arr; // `arr` is converted to a pointer pointing to its 0th element
    printf("intp is %p; address of the first element of arr is %p\n", intp,
           &arr[0]);

    // We can do pointer arithmetic on array thanks to array decaying
    printf("arr[2]=%d; *(arr + 2)=%d;\n", arr[2], *(arr + 2));

    // Array subscript is equivelant to pointer arithmetic
    // x[y] <=> *(x + y)
    // arr[1] => *(arr + 1) => *(1 + arr) => 1[arr]
    printf("This will work: %d\n", 1[arr]); // A weird way to demonstrate the equivelance

    // Exception 1: No array decaying when using sizeof
    // sizeof returns the size of the array in bytes
    printf("sizeof intp: %zu; sizeof arr: %zu\n", sizeof(intp), sizeof(arr));

    // Exception 2: No array decaying occurs when using &
    // p2 is a pointer to an ARRAY of 5 integers, instead of a pointer to int
    int (*p2)[5] = &arr;
    // Assume p2 = 0x100, p2+1=0x100+sizeof(arr)=0x114
    printf("p2=%p; p2+1=%p\n", p2, p2 + 1);

    // Exception 3: No array decaying during string initialization
    char s[] = "Test string";

    // Pointer casting
    int x = 0x12345678;
    char *c_ptr = (char *)&x;
    printf("%#x\n", *c_ptr); // On little-endian machines, it should print 0x78
    for (int i = 0; i < sizeof(int); i++) {
        printf("%p: byte %d: %#x\n", c_ptr + i, i, c_ptr[i]);
    }

    // Function pointer
    // To see how to understand these complex type definitions, check out
    // https://c-faq.com/decl/spiral.anderson.html
    void (*fp)(int) = print_int;
    fp(1234);

    // Function pointer in practice
    int unsorted_array[] = {5, 2, 8, 4, 9, 1};
    size_t nelem = sizeof(unsorted_array) / sizeof(unsorted_array[0]);
    qsort(unsorted_array, nelem, sizeof(unsorted_array[0]), compare_int);
    for (size_t i = 0; i < nelem; i++) {
        printf("%zu: %d\n", i, unsorted_array[i]);
    }
}
