#ifndef HAL_UART_DEBUG_H
#define HAL_UART_DEBUG_H
#include <stdint.h>
/* USART1 TX has one owner: this driver. Do not mix with HAL_UART_Transmit*. */
void uart_debug_init(void);
uint8_t uart_debug_write(const char *data,uint16_t length);
void uart_debug_irq(void);
void uart_debug_stats(uint32_t *dropped,uint32_t *sent);
/* Fatal path only: bounded best-effort drain, independent of tick/interrupts. */
void uart_debug_panic_flush(void);
#endif
