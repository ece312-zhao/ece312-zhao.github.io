#pragma once

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint64_t cycle_now() {
  unsigned long long t;
  __asm__ __volatile__("mfence\n\t"
                       "lfence\n\t"
                       "rdtsc\n\t"
                       "shl $32, %%rdx\n\t"
                       "or %%rdx, %0\n\t"
                       "lfence"
                       : "=a"(t)
                       :
                       : "rdx", "memory", "cc");
  return t;
}

static void init_array_range(int *arr, size_t sz) {
  for (size_t i = 0; i < sz; i++) {
    arr[i] = i;
  }
}

// exclusive
static int rand_int(unsigned max) {
  static bool seeded = false;
  if (!seeded) {
    srand(cycle_now());
    seeded = true;
  }

  if (max == 0)
    return 0;

  int max_num = (RAND_MAX / max) * max, num;
  do {
    num = rand();
  } while (num >= max_num);
  return num % max;
}

static void shuffle_int_arr(int *arr, size_t sz) {
  if (sz <= 1)
    return;

  for (size_t i = 0; i < sz; i++) {
    size_t rem = sz - i;
    size_t idx = rand_int(rem) + i;
    int tmp = arr[i];
    arr[i] = arr[idx];
    arr[idx] = tmp;
  }
}

static void print_arr(int *arr, size_t size) {
  for (size_t i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

static bool is_sorted(int *arr, size_t size) {
  if (size <= 1)
    return true;

  for (size_t i = 1; i < size; i++) {
    if (arr[i - 1] > arr[i]) {
      return false;
    }
  }
  return true;
}

typedef void (*SortAlgo)(int *, size_t);

static uint64_t _time_sorting_algorithm(SortAlgo algo, int *arr, size_t sz,
                                        size_t repeat, bool reshuffle) {
  uint64_t start, total = 0;

  for (size_t i = 0; i < repeat; i++) {
    if (reshuffle) {
      shuffle_int_arr(arr, sz);
    }

    start = cycle_now();
    algo(arr, sz);
    total += cycle_now() - start;

    assert(is_sorted(arr, sz));
  }
  return total / repeat;
}

#define MAX_BENCHMARK 100
#define MAX_ALGO_NAME 100

struct SortAlgoBench {
  SortAlgo func;
  const char *name;
};

static size_t _n_algo = 0;
static struct SortAlgoBench _bench[MAX_BENCHMARK];

#define register_algorithm(algo)                                               \
  do {                                                                         \
    if (_n_algo < MAX_BENCHMARK) {                                             \
      _bench[_n_algo].func = (algo);                                           \
      _bench[_n_algo].name = #algo;                                            \
      _n_algo += 1;                                                            \
    } else {                                                                   \
      fprintf(stderr, "Too many benchmarks\n");                                \
    }                                                                          \
  } while (0)

static void benchmark_sorting_algo_avg(int *arr, size_t sz, size_t repeat) {
  printf("\nAverage case:\n");
  for (size_t i = 0; i < _n_algo; i++) {
    uint64_t time =
        _time_sorting_algorithm(_bench[i].func, arr, sz, repeat, true);
    printf("\t%s: runtime=%" PRIu64 " cycles\n", _bench[i].name, time);
  }
}

static void benchmark_sorting_algo_best_case(int *arr, size_t sz,
                                             size_t repeat) {
  printf("\nBest case:\n");
  for (size_t i = 0; i < _n_algo; i++) {
    _bench[i].func(arr, sz);
    uint64_t time =
        _time_sorting_algorithm(_bench[i].func, arr, sz, repeat, false);
    printf("\t%s: runtime=%" PRIu64 " cycles\n", _bench[i].name, time);
  }
}
