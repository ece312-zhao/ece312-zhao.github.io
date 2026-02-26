#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sum_arr(int *arr, size_t len) {
  if (len == 0) return 0;
  int n = arr[0];
  // printf("%p\n", (void *)&n);
  return n + sum_arr(arr + 1, len - 1);
}

int sum_arr_tail_recursion(int *arr, size_t len, int partial_sum) {
  if (len == 0) return partial_sum;

  partial_sum += arr[0];
  return sum_arr_tail_recursion(arr + 1, len - 1, partial_sum);
}



int knapsack(int *weights, int *values, size_t n_items, int capacity) {
  if (n_items == 0) return 0;

  int skip = knapsack(weights + 1, values + 1, n_items - 1, capacity);
  if (weights[0] > capacity) {
    return skip;
  }

  int take = values[0] +
             knapsack(weights + 1, values + 1, n_items - 1, capacity - weights[0]);
  if (take > skip) {
    return take;
  } else {
    return skip;
  }
}

#define MAZE_R 5
#define MAZE_C 6
#define MAZE_C_BYTES (MAZE_C + 1)
static char map[MAZE_R][MAZE_C_BYTES] = {
  "x  x x",
  "xx xxx",
  " x x x",
  "   x  ",
  "xx xxx"
};

void print_map(char map[][MAZE_C_BYTES]) {
  printf("+------+\n");
  for (size_t i = 0; i < MAZE_R; i++) {
    printf("|%s|\n", map[i]);
  }
  printf("+------+\n");
}

void fill(char map[][MAZE_C_BYTES], int row, int col) {
  if (col < 0 || row < 0 || col > MAZE_C || row > MAZE_R) {
    return;
  }

  if (map[row][col] != ' ') return;

  map[row][col] = '*';
  fill(map, row, col - 1); // left
  fill(map, row, col + 1); // right
  fill(map, row - 1, col); // up
  fill(map, row + 1, col); // down
}

int main(int argc, char **argv) {
  if (argc < 2) return 1;

  size_t size = strtoul(argv[1], NULL, 10);
  int *arr = calloc(size, sizeof(*arr));
  if (!arr) return 2;

  for (size_t i = 0; i < size; i++) {
    arr[i] = i;
  }

  int sum = sum_arr_tail_recursion(arr, size, 0);
  printf("Array sum: %d\n\n", sum);
  free(arr);

  int weights[] = {100, 300, 200, 400, 50};
  int values[] = {10, 20, 20, 5, 12};
  const size_t n_items = sizeof(weights) / sizeof(weights[0]);
  printf("Knapsack under %d: %d\n", 300,
         knapsack(weights, values, n_items, 300));

  printf("Knapsack under %d: %d\n", 350,
         knapsack(weights, values, n_items, 350));

  printf("Knapsack under %d: %d\n", 500,
         knapsack(weights, values, n_items, 500));

  printf("Knapsack under %d: %d\n", 1000,
         knapsack(weights, values, n_items, 1000));

  print_map(map);
  fill(map, 3, 5);
  print_map(map);

  return 0;
}

size_t binary_search_recursive(int *arr, size_t low, size_t high, size_t sz,
                               int target) {
  if (low == high)
    return sz;

  size_t mid = low + (high - low) / 2;
  if (arr[mid] == target) {
    return mid;
  } else if (arr[mid] < target) {
    return binary_search_recursive(arr, mid + 1, high, sz, target);
  } else {
    return binary_search_recursive(arr, low, mid, sz, target);
  }
}
