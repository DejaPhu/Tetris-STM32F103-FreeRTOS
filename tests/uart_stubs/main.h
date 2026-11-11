#ifndef UART_TEST_MAIN_H
#define UART_TEST_MAIN_H
#include <stdint.h>
typedef struct { volatile uint32_t SR,DR,CR1; } MockUart;
extern MockUart mock_uart;
#define USART1 (&mock_uart)
#define USART_CR1_TXEIE (1U<<7)
#define USART_SR_TXE (1U<<7)
#define USART1_IRQn 37
uint32_t __get_PRIMASK(void);
void __disable_irq(void);
void __set_PRIMASK(uint32_t mask);
void HAL_NVIC_SetPriority(int irq,uint32_t priority,uint32_t sub);
void HAL_NVIC_EnableIRQ(int irq);
uint32_t HAL_GetTick(void);
#endif
