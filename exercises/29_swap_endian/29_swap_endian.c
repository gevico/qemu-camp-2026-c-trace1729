#include <stdint.h>
#include <stdio.h>

uint32_t swap_endian(uint32_t num) {
    // basically extract >> 1
    // first extract lower bits num & 0x000000ff
    // num & 0x0000ff00 >> 8
    // num & 0xff000000 >> 8 * 2
    uint32_t mask = 0x000000ff;
    uint32_t res = 0;
    for (int i = 0; i < 4; i++) {
        uint8_t byte = (num & mask) >> (8*i);
        res = (res << 8) + byte;
        mask <<= 8;
    }

    return res;
}


int main(int argc, char* argv[]) {
    uint32_t num = 0x78563412;
    uint32_t swapped = swap_endian(num);
    printf("0x%08x -> 0x%08x\n", num, swapped);
    return 0;
}
