#include "app_ipc.h"
#include "app_cfg.h"
QueueHandle_t xInputQueue = NULL;
QueueHandle_t xFrameQueue = NULL;
BaseType_t app_ipc_init(void)
{
    xInputQueue = xQueueCreate(INPUT_QUEUE_LENGTH, sizeof(InputEvent_t));
    xFrameQueue = xQueueCreate(FRAME_QUEUE_LENGTH, sizeof(GameFrame_t));
    return (xInputQueue != NULL && xFrameQueue != NULL) ? pdPASS : pdFAIL;
}
