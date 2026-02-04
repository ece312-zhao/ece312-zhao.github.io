#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


int main() {

    // safest malloc() call:
    int length = 100; // or size_t length
    int* my_ptr = malloc(length * sizeof(int));
    if (my_ptr == NULL) {
        printf("ERROR: malloc failed :(\n");
        return 1;
    }

    int* other_ptr = calloc(length, sizeof(int));


    free(my_ptr);
    my_ptr[5] = 10;
    printf("%p\n%d\n", my_ptr, my_ptr[5]);
    my_ptr = NULL;
    printf("%p\n", my_ptr);
    my_ptr[5] = 11;
    my_ptr = realloc(other_ptr, (length + 1) * sizeof(int));
    printf("%p\n%d\n", my_ptr, my_ptr[5]);
    other_ptr = NULL; // DON'T FREE OTHER_PTR! IF WE REALLOC, WE DON'T NEED TO FREE THE OLD ONE

    free(my_ptr); // we re-used my_ptr with the realloc, so we gotta free it again now
    my_ptr = NULL; // for extra safety

    /////////////////////////////////////////////////
    //// Below are some examples of errors and   ////
    //// leaks. To try them out, comment all out ////
    //// except for one that you want to try.    ////
    //// Then, compile and run with valgrind to  ////
    //// see what the error message looks like.  ////
    /////////////////////////////////////////////////

    /*-------- uninitialized memory example --------*/

    // int *x;
    // *x = 4;

    /*-------- invalid read or write example --------*/

    // int* x = malloc(sizeof(int));
    // x += 2; // we changed the pointer address to some invalid location

    // printf("%d\n", *x); // reading from invalid location
    // *x = 4; // writing to an invalid location

    // free(x-2); // this line is correct (but ugly) (seriously, don't code like this)

    /*-------- invalid free() example --------*/

    // int* x = malloc(sizeof(int));
    // free(x + 4); // freeing some random address
    // free(x); // this line is correct :)

    /*-------- memory leak example --------*/

    // int *x = malloc(sizeof(int));
    // *x = 4;
    // // no free() >:(


    return 0;

}