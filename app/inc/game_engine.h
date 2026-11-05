#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H
#include "app_ipc.h"
/* TODO(member 2): pure-C state machine, scoring, collision and rotation. */
void game_init(void);
void game_handle_input(InputCommand_t command);
void game_step(uint32_t elapsed_ms);
void game_build_frame(GameFrame_t *out);
#endif
