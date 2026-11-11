/* Small formatter avoids printf/newlib locks and large variadic stack frames. */
#include "main.h"
#include "hal_uart.h"
#include "app_log.h"
static void number(char *line,uint16_t *n,uint32_t value) {
    char digits[10];uint8_t i=0;
    do {digits[i++]=(char)('0'+value%10U);value/=10U;}while(value);
    while(i)line[(*n)++]=digits[--i];
}
void app_log(const char *event,uint32_t a,uint32_t b,uint32_t c) {
    char line[96];uint16_t n=0;uint8_t i=0;
    if(!event)return;
    line[n++]='t';line[n++]='=';number(line,&n,HAL_GetTick());line[n++]=' ';
    /* Event names are limited so every line fits even with UINT32_MAX values. */
    while(event[i] && i<24U)line[n++]=event[i++];
    line[n++]=' ';line[n++]='a';line[n++]='=';number(line,&n,a);
    line[n++]=' ';line[n++]='b';line[n++]='=';number(line,&n,b);
    line[n++]=' ';line[n++]='c';line[n++]='=';number(line,&n,c);
    line[n++]='\r';line[n++]='\n';
    (void)uart_debug_write(line,n);
}
