#include <stdint.h>
#include "tm4c123gh6pm.h"

void PLL_Init(void) {
    // 1. Use RCC2 for advanced features
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;

    // 2. Bypass PLL while initializing
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;

    // 3. Configure crystal and oscillator
    SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;       // clear XTAL field
    SYSCTL_RCC_R |= SYSCTL_RCC_XTAL_16MHZ;    // 16 MHz crystal
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;  // clear oscillator source
    SYSCTL_RCC2_R |= SYSCTL_RCC2_OSCSRC2_MO;  // main oscillator

    // 4. Activate PLL
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;

    // 5. Set DIV400 and system divider for 80 MHz
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;      // use 400 MHz PLL
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~SYSCTL_RCC2_SYSDIV2_M) | (4 << 22); // 400/5 = 80 MHz

    // 6. Wait for PLL to lock
    while((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0) {}

    // 7. Enable PLL by clearing BYPASS
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}
