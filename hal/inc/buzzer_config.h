#ifndef BUZZER_CONFIG_H
#define BUZZER_CONFIG_H
/* User's three-pin module: LOW-level trigger. HIGH must mean silent. */
#ifndef BUZZER_ACTIVE_LOW
#define BUZZER_ACTIVE_LOW 1U
#endif
#if BUZZER_ACTIVE_LOW
#define BUZZER_PWM_POLARITY TIM_OCPOLARITY_LOW
#else
#define BUZZER_PWM_POLARITY TIM_OCPOLARITY_HIGH
#endif
#endif
