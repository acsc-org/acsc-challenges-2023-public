#include <stdio.h>
#include <stdint.h>

uint8_t ENCFLAG[] = {0x01, 0x19, 0xef, 0x5a, 0xfa, 0xc8, 0x2e, 0x69, 0x31, 0xd7, 0x81, 0x21};

uint32_t reg = 0x3d2964f0;

uint32_t lfsr32()
{
    uint32_t mask = 0x80200003;
    uint32_t lsb = reg & 1;

    reg >>= 1;
    mask &= -1 * lsb;
    reg ^= mask;

    return reg;
}

int main()
{
    char flag[sizeof(ENCFLAG)];

    uint64_t loopnum = 1;
    for (int chidx = 0; chidx < (int)sizeof(ENCFLAG); chidx++) {
        uint8_t key;
        for (uint64_t i = 0; i < loopnum; i++) {
            key = lfsr32();
        }

        flag[chidx] = ENCFLAG[chidx] ^ key;

        loopnum *= 42;
        loopnum %= 0xffffffff;
    }

    printf("ACSC{%s}\n", flag);

    return 0;
}
