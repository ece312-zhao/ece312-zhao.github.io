#include "helper.h"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void swap_int(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

// Naive bubble sort
void bubble_sort_v0(int *arr, size_t sz) {
  if (sz <= 1) return;

  for (size_t i = 0; i < sz - 1; i++) {
    for (size_t j = 1; j < sz; j++) {
      if (arr[j] < arr[j - 1]) {
        swap_int(&arr[j - 1], &arr[j]);
      }
    }
  }
}

// Early termination that helps the best-case of already sorted list
void bubble_sort_v1(int *arr, size_t sz) {
  if (sz <= 1) return;

  for (size_t i = 0; i < sz - 1; i++) {
    bool swapped = false;
    for (size_t j = 1; j < sz; j++) {
      if (arr[j] < arr[j - 1]) {
        swap_int(&arr[j - 1], &arr[j]);
        swapped = true;
      }
    }

    if (!swapped) break;
  }
}

// Keep a sorted region so that we can reduce the number of inner loop iterations
void bubble_sort_v2(int *arr, size_t sz) {
  if (sz <= 1) return;

  for (size_t i = 0; i < sz - 1; i++) {
    bool swapped = false;
    for (size_t j = 1; j < sz - i; j++) {
      if (arr[j] < arr[j - 1]) {
        swap_int(&arr[j - 1], &arr[j]);
        swapped = true;
      }
    }

    if (!swapped) break;
  }
}

// More aggresively update the sorted region. Not very effective though
void bubble_sort_v3(int *arr, size_t sz) {
  if (sz <= 1) return;

  size_t end = sz;
  for (size_t i = 0; i < sz - 1; i++) {
    size_t last_swapped = 0;
    for (size_t j = 1; j < end; j++) {
      if (arr[j] < arr[j - 1]) {
        swap_int(&arr[j - 1], &arr[j]);
        last_swapped = j;
      }
    }
    if (!last_swapped) break;

    end = last_swapped;
  }
}

// A basic insertion sort using swapping
void insertion_sort_v0(int *arr, size_t sz) {
  if (sz <= 1) return;

  for (size_t sorted = 1; sorted < sz; sorted++) {
    for (size_t i = sorted; i > 0 && arr[i] < arr[i - 1]; i--) {
      swap_int(&arr[i], &arr[i - 1]);
    }
  }
}

// Insertion sort using move instead of swapping
void insertion_sort_v1(int *arr, size_t sz) {
  if (sz <= 1) return;

  for (size_t sorted = 1; sorted < sz; sorted++) {
    size_t elem = arr[sorted], i = sorted;
    while (i > 0 && elem < arr[i - 1]) {
      arr[i] = arr[i - 1];
      i -= 1;
    }

    if (i != sorted) {
      arr[i] = elem;
    }
  }
}

void quick_sort_v0(int *arr, size_t sz) {
  if (sz <= 1) return;

  int *left = calloc(sz, sizeof(int)), *right = calloc(sz, sizeof(int));
  assert(left && right);

  int pivot = arr[0];
  size_t l_idx = 0, r_idx = 0;
  for (size_t i = 1; i < sz; i++) {
    if (arr[i] < pivot) {
      left[l_idx++] = arr[i];
    } else {
      right[r_idx++] = arr[i];
    }
  }

  quick_sort_v0(left, l_idx);
  quick_sort_v0(right, r_idx);

  size_t cnt = 0;
  for (size_t i = 0; i < l_idx; i++) {
    arr[cnt++] = left[i];
  }
  arr[cnt++] = pivot;
  for (size_t i = 0; i < r_idx; i++) {
    arr[cnt++] = right[i];
  }

  free(left);
  free(right);
}

void quick_sort_v1(int *arr, size_t sz) {
  if (sz <= 1) return;

  int pivot = arr[0];
  size_t l_idx = 1, r_idx = sz - 1;
  while (true) {
    while (l_idx < sz && arr[l_idx] <= pivot) l_idx++;
    while (r_idx > 0 && arr[r_idx] > pivot) r_idx--;

    if (r_idx < l_idx) {
      break;
    }
    swap_int(&arr[l_idx], &arr[r_idx]);
  }
  swap_int(&arr[0], &arr[l_idx - 1]);

  quick_sort_v1(arr, l_idx - 1);
  quick_sort_v1(arr + l_idx, sz - l_idx);
}

void quick_sort_v2(int *arr, size_t sz) {
  if (sz <= 1) return;

  swap_int(&arr[0], &arr[rand_int(sz)]);

  int pivot = arr[0];
  size_t l_idx = 1, r_idx = sz - 1;
  while (true) {
    while (l_idx < sz && arr[l_idx] <= pivot) l_idx++;
    while (r_idx > 0 && arr[r_idx] > pivot) r_idx--;

    if (r_idx < l_idx) {
      break;
    }
    swap_int(&arr[l_idx], &arr[r_idx]);
  }
  swap_int(&arr[0], &arr[l_idx - 1]);

  quick_sort_v2(arr, l_idx - 1);
  quick_sort_v2(arr + l_idx, sz - l_idx);
}

int main(int argc, char **argv) {
  if (argc < 3) return 1;

  size_t size = strtoul(argv[1], NULL, 10);
  size_t repeat = strtoul(argv[2], NULL, 10);

  int *arr = calloc(size, sizeof(int));
  if (!arr) {
    fprintf(stderr, "Failed to allocate an array\n");
    return 1;
  }
  init_array_range(arr, size);

  if (size <= 32) {
    shuffle_int_arr(arr, size);
    print_arr(arr, size);
    quick_sort_v1(arr, size);
    print_arr(arr, size);
  }

  register_algorithm(bubble_sort_v0);
  register_algorithm(bubble_sort_v1);
  register_algorithm(bubble_sort_v2);
  register_algorithm(bubble_sort_v3);
  register_algorithm(insertion_sort_v0);
  register_algorithm(insertion_sort_v1);
  register_algorithm(quick_sort_v0);
  register_algorithm(quick_sort_v1);
  register_algorithm(quick_sort_v2);

  benchmark_sorting_algo_avg(arr, size, repeat);
  benchmark_sorting_algo_best_case(arr, size, repeat);

  free(arr);
  return 0;
}
