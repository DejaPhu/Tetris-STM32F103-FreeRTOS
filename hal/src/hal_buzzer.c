/* Nonblocking PWM effects. TIM4: 72 MHz / (71+1) = 1 MHz. */
#include "main.h"
#include "hal_buzzer.h"
#include "buzzer_config.h"
extern TIM_HandleTypeDef htim4;
static uint32_t sound_deadline;
static uint8_t sound_active;
/* Preload output data before selecting GPIO mode: avoid a LOW pulse at boot.
 * Direct register access keeps this path bounded, including in a fault hook. */
void buzzer_hold_idle(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
#if BUZZER_ACTIVE_LOW
    GPIOB->BSRR=GPIO_PIN_6;
#else
    GPIOB->BRR=GPIO_PIN_6;
#endif
    MODIFY_REG(GPIOB->CRL,GPIO_CRL_MODE6|GPIO_CRL_CNF6,GPIO_CRL_MODE6_0);
}
uint8_t buzzer_init(void) {
    GPIO_InitTypeDef pin={0};
    buzzer_hold_idle();
    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,0U);
    if(HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1)!=HAL_OK) return 0U;
    /* Timer is already running at inactive level before PB6 joins TIM4_CH1. */
    pin.Pin=GPIO_PIN_6;pin.Mode=GPIO_MODE_AF_PP;
    pin.Speed=GPIO_SPEED_FREQ_LOW;HAL_GPIO_Init(GPIOB,&pin);
    return 1U;
}
void tone(uint16_t hz, uint32_t ms)
{
    uint32_t period;
    if (!hz) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0U);
        sound_active = 0U;
        return;
    }
    /* CubeMX preset: TIM4 input 72 MHz, PSC=71 -> timer tick 1 MHz. */
    period = 1000000UL / hz;
    if (period < 2U) period = 2U;
    if (period > 65536UL) period=65536UL; /* TIM4 is 16-bit. */
    __HAL_TIM_SET_AUTORELOAD(&htim4, period - 1U);
    __HAL_TIM_SET_COUNTER(&htim4, 0U);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, period / 2U);
    sound_deadline = HAL_GetTick() + ms;
    sound_active = 1U;
}

void tone_poll(void)
{
    if (sound_active && (int32_t)(HAL_GetTick() - sound_deadline) >= 0) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0U);
        sound_active = 0U;
    }
}
