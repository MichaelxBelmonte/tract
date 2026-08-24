#include <stdint.h>
#include <stdio.h>
#include <sys/auxv.h>

#define HWCAP2_SME (1ULL << 23)
#define HWCAP2_SME2 (1ULL << 37)
#define HWCAP2_SME_F16F16 (1ULL << 42)

int main(void) {
    const unsigned long long hwcap2 = getauxval(AT_HWCAP2);
    register uint64_t x0 __asm__("x0");

    if ((hwcap2 & HWCAP2_SME) == 0) {
        fprintf(stderr, "SME is missing; HWCAP2=0x%llx\n", hwcap2);
        return 2;
    }

    __asm__ volatile(".inst 0x04bf5820" : "=r"(x0));
    printf(
        "HWCAP2=0x%llx SME=%d SME2=%d SME_F16F16=%d SVL_BYTES=%llu\n",
        hwcap2,
        (hwcap2 & HWCAP2_SME) != 0,
        (hwcap2 & HWCAP2_SME2) != 0,
        (hwcap2 & HWCAP2_SME_F16F16) != 0,
        (unsigned long long)x0
    );

    return (hwcap2 & HWCAP2_SME_F16F16) != 0 && x0 == 64 ? 0 : 3;
}
