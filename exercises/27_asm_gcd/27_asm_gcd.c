#include <stdio.h>

/*
 * __asm__ mental model
*     1. argumenet list (: "=r" (var0), : "r" (var1))
*     2. reference arguments in asm ($0, $1)
*     3. scope (: "eax" )
 * */

unsigned int gcd_asm(unsigned int a, unsigned int b) {
    unsigned int result;

    /*
     * a, b <- b, a % b
     * b, a - n*b,
     * */

    __asm__ volatile (
        "mov %1, %%eax\n\t"     // 将a存入eax
        "mov %2, %%ebx\n\t"     // 将b存入ebx
        "jmp .L_check\n\t"      // 跳转到检查条件

        ".L_loop:\n\t"
        "   xor %%edx, %%edx\n\t"
        "   div %%ebx\n\t"
        "   mov %%ebx, %%eax\n\t"
        "   mov %%edx, %%ebx\n\t"

        ".L_check:\n\t"
        "   test %%ebx, %%ebx\n\t"      // 测试ebx是否为0
        "   jne .L_loop\n\t"    // 如果不为0，继续循环

        "mov %%eax, %0"         // 将结果（eax）存入输出变量
        : "=r" (result)           // 输出操作数
        : "r" (a), "r" (b)      // 输入操作数
        : "eax", "ebx", "edx", "cc" // 被修改的寄存器
    );

    return result;
}

int main(int argc, char* argv[]) {
    printf("%d\n", gcd_asm(12, 8));
    printf("%d\n", gcd_asm(7, 5));
    return 0;
}
