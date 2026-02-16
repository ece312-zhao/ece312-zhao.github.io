#pragma once
#include <stdint.h>

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
