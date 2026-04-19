#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    carry ? "Overflow" : "Not Overflow"

/*
 * introduce the direct alias
 * paramter: [alias] "priviledge" (var)
 * asm: %[alias]
 * "a/b/c/d" bind value to ea/b/c/dx register group, to avoid manuall binding
 * */

int check_add_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int sum = a;
    unsigned char carry;

    __asm__ volatile(
        "addl %[rhs], %[lhs]\n\t"
        "setc %[carry]"
        : [lhs] "+r" (sum), [carry] "=qm" (carry)
        : [rhs] "r" (b)
        : "cc"
    );

    return carry;
}

int check_sub_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int diff = a;
    unsigned char carry;

    __asm__ volatile(
        "subl %[rhs], %[lhs]\n\t"
        "setc %[carry]"
        : [lhs] "+r" (diff), [carry] "=qm" (carry)
        : [rhs] "r" (b)
        : "cc"
    );

    return carry;
}

int check_mul_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int low_bits;
    unsigned int high_bits;
    unsigned char overflow;

    __asm__ volatile(
        "mull %[rhs]\n\t"
        "setc %[overflow]"
        : "=a" (low_bits), "=d" (high_bits), [overflow] "=qm" (overflow)
        : "a" (a), [rhs] "r" (b)
        : "cc"
    );

    (void)low_bits;
    return overflow || (high_bits != 0);
}

int check_div_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char is_div_zero;

    __asm__ volatile(
        "testl %[divisor], %[divisor]\n\t"
        "setz %[flag]"
        : [flag] "=qm" (is_div_zero)
        : [divisor] "r" (b), "r" (a)
        : "cc"
    );

    return is_div_zero;
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));   // 1
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));
    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));          // 1
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));
    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));   // 1
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));
    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));                         // 1
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));
    return 0;
}
