#include <stdlib.h>
/* AHB1 Base Addresses ******************************************************/
#define STM32_RCC_BASE 0x40023800 /* 0x40023800-0x40023bff: Reset
and Clock control RCC */
/* Register Offsets *********************************************************/
#define STM32_RCC_AHB1ENR_OFFSET 0x0030 /* AHB1 Peripheral Clock enable
register */
/* Register Addresses *******************************************************/
#define STM32_RCC_AHB1ENR (STM32_RCC_BASE+STM32_RCC_AHB1ENR_OFFSET)
/* AHB1 Peripheral Clock enable register */
#define RCC_AHB1ENR_GPIOCEN (1 << 2) /* Bit 2: IO port C clock
enable */

int main(int argc, char *argv[])
{
    while(1){};
    /* Nao deveria chegar aqui */
    return EXIT_SUCCESS;
}