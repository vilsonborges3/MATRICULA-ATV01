#include <stdlib.h>
#include <stdint.h>

#define LED_DELAY 50000
#define PINO_LED 13
/* changes to the F103 */

#define STM32_RCC_BASE      0x40021000     /* 0x40021000 - 0x400213ff: Reset and Clock control RCC */

/* AHB2 Base Addresses ******************************************************/

#define STM32_GPIOC_BASE    0x40011000     /* 0x40011000 - 0x400113ff: GPIO Port C */

/* Register Offsets *********************************************************/
#define STM32_RCC_APB2ENR_OFFSET    0x0018  /* APB2 peripheral clock enable register */
#define STM32_GPIO_CRL_OFFSET       0x0000  /* Port configuration register low */
#define STM32_GPIO_CRH_OFFSET       0x0004  /* Port configuration register high */
#define STM32_GPIO_BSRR_OFFSET      0x0010  /* Port bit set/reset register */


/* Register Addresses *******************************************************/
#define STM32_RCC_APB2ENR               (STM32_RCC_BASE+STM32_RCC_APB2ENR_OFFSET)
#define STM32_GPIOC_CRL                 (STM32_GPIOC_BASE+STM32_GPIO_CRL_OFFSET)
#define STM32_GPIOC_CRH                 (STM32_GPIOC_BASE+STM32_GPIO_CRH_OFFSET)
#define STM32_GPIOC_BSRR                (STM32_GPIOC_BASE+STM32_GPIO_BSRR_OFFSET)

/* APB2 Peripheral Clock enable register (RCC_APB2ENR) */
#define RCC_APB2ENR_IOPCEN         (1 << 4)    /* Bit 4 IOPCEN: IO port C clock enable */

/* Port configuration register */
#define GPIO_CNF_I_ANALOG_MODE        (0) /* Input: Analog mode */
#define GPIO_CNF_I_FLOATING_INPUT     (1) /* Input: Floating input (reset state) */
#define GPIO_CNF_I_PULL_UP_DOWN       (2) /* Input: Input with pull-up / pull-down */
                                      /* Input 11 (3): Reserved */
#define GPIO_CNF_O_GPO_PUSH_PULL      (0) /* Output: General purpose output push-pull */
#define GPIO_CNF_O_GPO_OPEN_DRAIN     (1) /* Output: General purpose output Open-drain */
#define GPIO_CNF_O_AFO_PUSH_PULL      (2) /* Output: Alternate function output Push-pull */
#define GPIO_CNF_O_AFO_OPEN_DRAIN     (3) /* Output: Alternate function output Open-drain */
#define GPIO_MODE_INPUT               (0) /* Input */
#define GPIO_MODE_OUTPUT_10MHZ        (1) /* Output 10MHz */
#define GPIO_MODE_OUTPUT_2MHZ         (2) /* Output 2MHz */
#define GPIO_MODE_OUTPUT_50MHZ        (3) /* Output 50MHz */

/* Deslocamento do registrador e Máscaras para setar os registradores da memória */

#define GPIO_MODE_SHIFT(n)            ((n < 8) ? (n << 2) : ((n - 8) << 2))
#define GPIO_MODE_MASK(n)             (3 << GPIO_MODE_SHIFT(n))
#define GPIO_CNF_SHIFT(n)             ((n < 8) ? ((n << 2) + 2) : (((n - 8) << 2) + 2))
#define GPIO_CNF_MASK(n)              (3 << GPIO_CNF_SHIFT(n))
/* GPIO port bit set/reset register */
#define GPIO_BSRR_SET(n)              (1 << (n))
#define GPIO_BSRR_RST(n)              (1 << (n + 16))

static uint32_t led_status;
static char fw_version[] = {'V', '1', '.', '0'};

int main(int argc, char *argv[])
{
    uint32_t reg;
    uint32_t i;
    /* Ponteiros para registradores */
    uint32_t *pRCC_APB2ENR  = (uint32_t *)STM32_RCC_APB2ENR;
    uint32_t *pGPIOC_CRL    = (uint32_t *)STM32_GPIOC_CRL;
    uint32_t *pGPIOC_CRH    = (uint32_t *)STM32_GPIOC_CRH;
    uint32_t *pGPIOC_BSRR   = (uint32_t *)STM32_GPIOC_BSRR;

    /* Habilita clock GPIOC */
    reg  = *pRCC_APB2ENR;
    reg |= RCC_APB2ENR_IOPCEN;
    *pRCC_APB2ENR = reg;

    reg = *pGPIOC_CRH;
    reg &= ~GPIO_CNF_MASK(PINO_LED);
    reg |= (GPIO_CNF_O_GPO_PUSH_PULL << GPIO_CNF_SHIFT(PINO_LED));

    reg &= ~GPIO_MODE_MASK(PINO_LED);
    reg |= (GPIO_MODE_OUTPUT_10MHZ << GPIO_MODE_SHIFT(PINO_LED));

    if(PINO_LED < 8) *pGPIOC_CRL = reg;
    else *pGPIOC_CRH = reg;
    while(1)
    {
        *pGPIOC_BSRR = GPIO_BSRR_SET(PINO_LED);
        for(i = 0; i < LED_DELAY; i++);
        *pGPIOC_BSRR = GPIO_BSRR_RST(PINO_LED);
        for(i = 0; i < LED_DELAY; i++);
    }
    /* Nao deveria chegar aqui */
    return EXIT_SUCCESS;
}