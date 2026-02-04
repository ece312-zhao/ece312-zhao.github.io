#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct {
    double *_store;
    size_t _cap, sz;
} Stack;

Stack *stackAlloc() {
    const size_t init_sz = 16;

    Stack *stack = malloc(sizeof(*stack));
    if (!stack) {
        return NULL;
    }

    stack->_store = calloc(init_sz, sizeof(double));
    if (!stack->_store) {
        free(stack);
        return NULL;
    }

    stack->_cap = init_sz;
    stack->sz = 0;
    return stack;
}

void stackFree(Stack *this) {
    if (!this) return;

    free(this->_store);
    free(this);
}

double stackPop(Stack *this) {
    assert(this && this->sz > 0);
    this->sz -= 1;
    return this->_store[this->sz];
}

bool stackPush(Stack *this, double val) {
    assert(this);
    if (this->sz >= this->_cap) {
        const size_t scale = 2;
        if (this->_cap > SIZE_MAX / sizeof(double) / scale) {
            return false;
        }
        size_t new_cap = this->_cap * scale;

        double *new_store = realloc(this->_store, sizeof(double) * new_cap);
        if (!new_store) {
            return false;
        }
        this->_store = new_store;
        this->_cap = new_cap;
    }
    this->_store[this->sz] = val;
    this->sz += 1;
    return true;
}

int main(int argc, char **argv) {
    if (argc < 2) return 1;

    Stack *stack = stackAlloc();
    if (!stack) {
        printf("Out of memory!\n");
        return 1;
    }

    int ret = 0;
    for (size_t i = 1; i < argc; i++) {
        if (strlen(argv[i]) == 1 && (argv[i][0] == '+' || argv[i][0] == '-' ||
                                     argv[i][0] == '*' || argv[i][0] == '/')) {
            // We are dealing with an operator
            if (stack->sz < 2) {
                printf("Insufficient numbers on the stack!\n");
                ret = 1;
                goto err;
            }

            double right = stackPop(stack), left = stackPop(stack), res;
            switch (argv[i][0]) {
                case '+': {
                    res = left + right;
                    break;
                }
                case '-': {
                    res = left - right;
                    break;
                }
                case '/': {
                    res = left / right;
                    break;
                }
                case '*': {
                    res = left * right;
                    break;
                }
                default: {
                    assert(false && "Illegal operator!");
                }
            }
            if (!stackPush(stack, res)) {
                printf("Out of memory\n");
                ret = 1;
                goto err;
            }
        } else {
            char *end;
            double num = strtod(argv[i], &end);
            if (argv[i] == end || *end != '\0') {
                printf("Failed to parse number \"%s\"\n", argv[i]);
                ret = 1;
                goto err;
            }

            if (!stackPush(stack, num)) {
                printf("Out of memory\n");
                ret = 1;
                goto err;
            }
        }
    }

    if (stack->sz != 1) {
        printf("Stack has zero or multiple values remaining!\n");
        ret = 1;
        goto err;
    }

    printf("%f\n", stackPop(stack));

err:
    stackFree(stack);
    return ret;
}
