/* Nonblocking PWM effects. TIM4: 72 MHz / (71+1) = 1 MHz. */
#include "main.h"
#include "hal_buzzer.h"
extern TIM_HandleTypeDef htim4;
static uint32_t sound_deadline;
static uint8_t sound_active;
uint8_t buzzer_init(void) {
    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,0U);
    return HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1)==HAL_OK;
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
