#ifndef TEST_BUZZER_MAIN_H
#define TEST_BUZZER_MAIN_H
#include <stdint.h>
typedef struct {uint32_t CCR,ARR,CNT;} TIM_HandleTypeDef;
typedef struct {uint32_t BSRR,BRR,CRL;} GPIO_TypeDef;
typedef struct {uint32_t Pin,Mode,Speed;} GPIO_InitTypeDef;
extern GPIO_TypeDef gpio_b;
#define GPIOB (&gpio_b)
#define GPIO_PIN_6 (1U<<6)
#define GPIO_CRL_MODE6 (3U<<24)
#define GPIO_CRL_CNF6 (12U<<24)
#define GPIO_CRL_MODE6_0 (1U<<24)
#define MODIFY_REG(reg,clear,set) ((reg)=((reg)&~(clear))|(set))
#define __HAL_RCC_GPIOB_CLK_ENABLE() ((void)0)
#define GPIO_MODE_AF_PP 11U
#define GPIO_SPEED_FREQ_LOW 2U
#define TIM_CHANNEL_1 0U
#define TIM_OCPOLARITY_HIGH 0U
#define TIM_OCPOLARITY_LOW 2U
#define HAL_OK 0U
#define __HAL_TIM_SET_COMPARE(t,ch,n) ((t)->CCR=(n))
#define __HAL_TIM_SET_AUTORELOAD(t,n) ((t)->ARR=(n))
#define __HAL_TIM_SET_COUNTER(t,n) ((t)->CNT=(n))
uint32_t HAL_TIM_PWM_Start(TIM_HandleTypeDef *t,uint32_t ch);
void HAL_GPIO_Init(GPIO_TypeDef *p,GPIO_InitTypeDef *cfg);
uint32_t HAL_GetTick(void);
#endif
