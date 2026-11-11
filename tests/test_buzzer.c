#include <assert.h>
#include <stdio.h>
#include "main.h"
#include "hal_buzzer.h"
#include "buzzer_config.h"
TIM_HandleTypeDef htim4;
GPIO_TypeDef gpio_b;
static uint32_t now,fail,started,af;
uint32_t HAL_GetTick(void) {return now;}
uint32_t HAL_TIM_PWM_Start(TIM_HandleTypeDef *t,uint32_t ch) {
 assert(t==&htim4&&ch==0&&t->CCR==0);
 assert(gpio_b.BSRR==GPIO_PIN_6);
 if(fail)return 1;started=1;return HAL_OK;
}
void HAL_GPIO_Init(GPIO_TypeDef *p,GPIO_InitTypeDef *cfg) {
 assert(started);assert(p==GPIOB&&cfg->Pin==GPIO_PIN_6&&cfg->Mode==GPIO_MODE_AF_PP);
 assert(htim4.CCR==0);af=1;
}
int main(void) {
 assert(BUZZER_PWM_POLARITY==TIM_OCPOLARITY_LOW);
 gpio_b.CRL=0xffffffffU;buzzer_hold_idle();
 assert(gpio_b.BSRR==GPIO_PIN_6);
 assert((gpio_b.CRL&(GPIO_CRL_MODE6|GPIO_CRL_CNF6))==GPIO_CRL_MODE6_0);
 fail=1;assert(!buzzer_init()&&!af);
 fail=0;assert(buzzer_init()&&af&&htim4.CCR==0);
 now=100;tone(1000,35);assert(htim4.ARR==999&&htim4.CCR==500&&htim4.CNT==0);
 now=134;tone_poll();assert(htim4.CCR==500);
 now=135;tone_poll();assert(htim4.CCR==0);
 tone(1500,100);tone(0,0);assert(htim4.CCR==0);
 now=0xfffffff0U;tone(900,32);now=15;tone_poll();assert(htim4.CCR>0);
 now=16;tone_poll();assert(htim4.CCR==0);
 tone(1,10);assert(htim4.ARR==65535);buzzer_hold_idle();
 assert(gpio_b.BSRR==GPIO_PIN_6);
 puts("PASS: LOW-trigger polarity, GPIO idle before PWM/after failure, timed mute, wrap and 16-bit timer limit");
}
