#ifndef GAME_INTERNAL_H
#define GAME_INTERNAL_H
/* Private to GameTask and its engine/FSM. Display uses snapshots only. */
#include "app_shared.h"
typedef struct {
    uint8_t board[TETRIS_ROWS][TETRIS_COLS];
    int8_t piece_x;
    int8_t piece_y;
    uint8_t current_piece;
    uint8_t next_piece;
    uint16_t mask;
    uint32_t score;
    uint16_t lines;
    uint32_t last_fall_tick;
    uint8_t menu_index;
    GameState_t state;
    Difficulty_t difficulty;
} Engine_t;

extern Engine_t game;
uint32_t app_now_ms(void);
void tone(uint16_t hz, uint32_t ms);
uint8_t occupied(uint16_t mask, uint8_t row, uint8_t col);
uint32_t gravity_ms(void);
uint8_t try_move(int8_t dx, int8_t dy);
uint8_t try_rotate(void);
void start_game(void);
void lock_piece(void);
void game_init(uint32_t seed);
uint8_t handle_button(uint16_t raw);
uint8_t game_tick(uint32_t now);
uint32_t game_wait_ms(uint32_t now);
#endif
