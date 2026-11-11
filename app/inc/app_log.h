#ifndef APP_LOG_H
#define APP_LOG_H
#include <stdint.h>
/* Three unsigned values, one timestamped line; task-context calls only. */
void app_log(const char *event,uint32_t a,uint32_t b,uint32_t c);
#endif
