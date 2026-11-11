#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "hal_uart.h"
#include "app_log.h"
MockUart mock_uart;
static uint32_t mask,now;
uint32_t __get_PRIMASK(void) { return mask; }
void __disable_irq(void) {mask=1U;}
void __set_PRIMASK(uint32_t value) {mask=value;}
void HAL_NVIC_SetPriority(int irq,uint32_t p,uint32_t sub) {assert(irq==37&&p==6&&sub==0);}
void HAL_NVIC_EnableIRQ(int irq) {assert(irq==37);}
uint32_t HAL_GetTick(void) {return now;}
static unsigned drain(char *out) {
 unsigned n=0;
 while(mock_uart.CR1&USART_CR1_TXEIE) {
  assert(n<512);mock_uart.SR=USART_SR_TXE;uart_debug_irq();out[n++]=(char)mock_uart.DR;
 }
 out[n]=0;return n;
}
int main(void) {
 char data[128],output[513];unsigned i;uint32_t lost,sent;
 assert(!uart_debug_write("early",5));uart_debug_init();
 assert(!(mock_uart.CR1&USART_CR1_TXEIE));
 memset(data,'x',sizeof data);
 for(i=0;i<3;i++)assert(uart_debug_write(data,128));
 assert(uart_debug_write(data,127));assert(!uart_debug_write("y",1));
 uart_debug_stats(&lost,&sent);assert(lost==1&&sent==0);
 assert(drain(output)==511);for(i=0;i<511;i++)assert(output[i]=='x');
 uart_debug_stats(&lost,&sent);assert(sent==511);
 for(i=0;i<1000;i++) {
  assert(uart_debug_write("abc",3));assert(drain(output)==3);assert(!strcmp(output,"abc"));
 }
 mask=1;assert(uart_debug_write("z",1));assert(mask==1);drain(output);mask=0;
 now=123;app_log("KEY_RX",6,4,0);drain(output);
 assert(!strcmp(output,"t=123 KEY_RX a=6 b=4 c=0\r\n"));
 now=0xffffffffU;app_log("ABCDEFGHIJKLMNOPQRSTUVWXYZ",0xffffffffU,0xffffffffU,0xffffffffU);
 assert(drain(output)<96);assert(strstr(output,"t=4294967295 ABCDEFGHIJKLMNOPQRSTUVWX a=4294967295"));
 assert(strstr(output,"c=4294967295\r\n"));
 /* Stalled hardware cannot make the fatal-path flush loop unbounded. */
 assert(uart_debug_write("pending",7));mock_uart.SR=0;uart_debug_panic_flush();assert(mask==0);
 assert(drain(output)==7);
 assert(!uart_debug_write(data,129));
 puts("PASS: UART full/overflow/wrap, complete lines, IRQ drain, saved interrupt mask, formatter limits, bounded fatal flush");
 return 0;
}
