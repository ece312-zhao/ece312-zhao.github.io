#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

// Protocol: If we find the target: return the index
// Else: return size
size_t linear_search(int *arr, size_t sz, int target) {
    for (size_t i = 0; i < sz; i++) {
        if (arr[i] == target) {
            return i;
        }
    }
    return sz;
}

size_t binary_search(int *arr, size_t sz, int target) {
    // The search range is [low, high)
    size_t low = 0, high = sz;
    while (low < high) {
        // This may overflow
        // size_t mid = (low + high) / 2;
        size_t mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            low = mid + 1;
        } else {
            // arr[mid] > target
            high = mid;
        }
    }
    return sz; // not found
}

int main(int argc, char **argv) {
    if (argc < 3) return 1;

    int size = strtol(argv[1], NULL, 10);
    int target = strtol(argv[2], NULL, 10);

    int *arr = calloc(size, sizeof(*arr));
    if (!arr) return 2;
    for (size_t i = 0; i < size; i++) {
        arr[i] = i;
    }

    size_t start, end;

    start = cycle_now();
    size_t idx = linear_search(arr, size, target);
    end = cycle_now();
    printf("Linear search: idx=%zu; found=%d; time=%zu cycles\n", idx,
           idx != size, end - start);

    start = cycle_now();
    idx = binary_search(arr, size, target);
    end = cycle_now();
    printf("Binary search: idx=%zu; found=%d; time=%zu cycles\n", idx,
           idx != size, end - start);

    free(arr);
    return 0;
}
