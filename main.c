/**
 * Infinite Power of Two Calculator
 *
 * This program continuously computes and prints powers of two:
 * 2^0, 2^1, 2^2, 2^3, ... forever (until manually terminated).
 *
 * It uses dynamic big integer arithmetic (base 10 digits) to handle
 * arbitrarily large results without overflow.
 *
 * Compile: gcc -o pow2 infinite_pow2.c
 * Run: ./pow2
 * Stop: Press Ctrl+C
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Initial capacity for digit array (grows dynamically) */
#define INIT_CAP 16

/* Big integer structure: little-endian decimal digits */
typedef struct {
    unsigned char *digits;   /* each digit 0-9, least significant first */
    int len;                 /* number of digits currently used */
    int capacity;            /* allocated size of digits array */
} BigInt;

/* Initialize a big integer with value 1 */
void bigint_init_one(BigInt *num) {
    num->capacity = INIT_CAP;
    num->digits = (unsigned char*)malloc(num->capacity * sizeof(unsigned char));
    if (!num->digits) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    num->digits[0] = 1;
    num->len = 1;
}

/* Free memory used by big integer */
void bigint_free(BigInt *num) {
    free(num->digits);
    num->digits = NULL;
    num->len = 0;
    num->capacity = 0;
}

/* Multiply big integer by 2 (in-place) */
void bigint_multiply_by_2(BigInt *num) {
    int carry = 0;
    for (int i = 0; i < num->len; i++) {
        int val = num->digits[i] * 2 + carry;
        num->digits[i] = val % 10;
        carry = val / 10;
    }
    /* If there is a final carry, extend the array */
    if (carry) {
        if (num->len >= num->capacity) {
            num->capacity *= 2;
            num->digits = (unsigned char*)realloc(num->digits, num->capacity * sizeof(unsigned char));
            if (!num->digits) {
                fprintf(stderr, "Memory reallocation failed.\n");
                exit(1);
            }
        }
        num->digits[num->len] = carry;
        num->len++;
    }
}

/* Convert big integer to printable decimal string (allocates new memory) */
char *bigint_to_string(const BigInt *num) {
    char *str = (char*)malloc((num->len + 1) * sizeof(char));
    if (!str) return NULL;
    for (int i = 0; i < num->len; i++) {
        str[num->len - 1 - i] = num->digits[i] + '0';
    }
    str[num->len] = '\0';
    return str;
}

int main() {
    BigInt current;
    bigint_init_one(&current);
    int exponent = 0;

    printf("=== Infinite Powers of Two ===\n");
    printf("Press Ctrl+C to stop.\n\n");

    /* Infinite loop – computes and prints 2^exponent forever */
    while (1) {
        char *str = bigint_to_string(&current);
        if (str) {
            printf("2^%d = %s\n", exponent, str);
            free(str);
        } else {
            printf("2^%d = [conversion error]\n", exponent);
        }

        /* Prepare next power: multiply current by 2 */
        bigint_multiply_by_2(&current);
        exponent++;
    }

    /* Unreachable, but clean up for completeness */
    bigint_free(&current);
    return 0;
}