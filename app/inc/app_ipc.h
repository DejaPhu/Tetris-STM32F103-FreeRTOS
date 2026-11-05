#ifndef APP_IPC_H
#define APP_IPC_H
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#define BOARD_ROWS 20
#define BOARD_COLS 10
typedef enum {
    CMD_LEFT, CMD_RIGHT, CMD_ROTATE, CMD_SOFT_DROP,
    CMD_HARD_DROP, CMD_PAUSE
} InputCommand_t;
typedef struct {
    InputCommand_t command;
    TickType_t tick;
} InputEvent_t;
typedef struct {
    uint8_t cells[BOARD_ROWS][BOARD_COLS];
    uint8_t next_cells[4][4];
    uint32_t score;
    uint16_t lines;
    uint8_t level;
    uint8_t paused;
    uint8_t game_over;
} GameFrame_t;
extern QueueHandle_t xInputQueue;
extern QueueHandle_t xFrameQueue;
BaseType_t app_ipc_init(void);
#endif
