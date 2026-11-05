#ifndef APP_CFG_H
#define APP_CFG_H
/* Starting values. Validate task timing and stack usage on target. */
#define INPUT_PERIOD_MS        10U
#define GAME_PERIOD_MS         20U
#define DISPLAY_PERIOD_MS      33U
#define INPUT_TASK_PRIORITY     3U
#define GAME_TASK_PRIORITY      2U
#define DISPLAY_TASK_PRIORITY   1U
/* xTaskCreate stack depths are measured in words. */
#define INPUT_STACK_WORDS     192U
#define GAME_STACK_WORDS      320U
#define DISPLAY_STACK_WORDS   384U
#define INPUT_QUEUE_LENGTH     16U
#define FRAME_QUEUE_LENGTH      1U
#endif
