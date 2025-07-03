#include "Bootloader.h"

char message_buffer[] = "Bootloader Mode\n\r";

// struct boot_vectable_ {
//     uint32_t Initial_SP;
//     void (*Reset_Handler)(void);
// };
// #ifdef BOOT_ADDR
//     #define BOOTVTAB	((struct boot_vectable_ *)BOOT_ADDR)
// #endif
void EnterBootloader(void)
{
#ifdef BOOT_ADDR

#ifdef STM32F1xx // If F103 (most probably base IntroSat) - print message to UART1
	

    for (char *next_char = message_buffer; *next_char != 0; next_char++)
    {
        while (READ_BIT(USART1->SR, USART_SR_TXE) != (USART_SR_TXE))
        {
        }
        USART1->DR = *next_char;
    }

    HAL_Delay(200);

    CLEAR_BIT(USART1->CR1, (USART_CR1_UE));
#endif /** STM32F103xx */

    /* Disable all interrupts */
	__disable_irq();

#ifdef USE_HAL_DRIVER
	HAL_RCC_DeInit();
#endif /** USE_HAL_DRIVER */
	
/* Disable Systick timer */
	SysTick->CTRL = 0;
  	SysTick->LOAD = 0;
  	SysTick->VAL = 0;

	/* Set the clock to the default state */


	/* Clear Interrupt Enable Register & Interrupt Pending Register */
	for (uint8_t i = 0; i < sizeof(NVIC->ICER) / sizeof(NVIC->ICER[0]); i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}

	/* Re-enable all interrupts */
	__enable_irq();
	void (*SysMemBootJump)(void);
	SysMemBootJump = (void (*)(void)) (*((uint32_t *) (BOOT_ADDR + 4)));
  	__set_MSP(*(uint32_t *)BOOT_ADDR);
  	SysMemBootJump();
#endif /** BOOT_ADDR */ 
}