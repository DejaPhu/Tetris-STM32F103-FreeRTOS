/* MEMBER 2: Tetris state machine + game rules + nonblocking TIM4 buzzer.
 * Owns all mutable game state. Publishes display snapshot and a uint16_t
 * frame notification through the existing CMSIS-RTOS2 1-item queue.
 */
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "../inc/app_shared.h"

extern TIM_HandleTypeDef htim4;

#define TILE(r,c) ((uint16_t)(1U << ((r)*4U + (c))))
#define SHAPE(a,b,c,d) ((uint16_t)((a)|(b)|(c)|(d)))

/* 4x4 local masks: 7 tetrominoes in order I,O,T,S,Z,J,L. */
static const uint16_t piece_masks[7] = {
    SHAPE(TILE(1,0),TILE(1,1),TILE(1,2),TILE(1,3)),
    SHAPE(TILE(0,1),TILE(0,2),TILE(1,1),TILE(1,2)),
    SHAPE(TILE(0,1),TILE(1,0),TILE(1,1),TILE(1,2)),
    SHAPE(TILE(0,1),TILE(0,2),TILE(1,0),TILE(1,1)),
    SHAPE(TILE(0,0),TILE(0,1),TILE(1,1),TILE(1,2)),
    SHAPE(TILE(0,0),TILE(1,0),TILE(1,1),TILE(1,2)),
    SHAPE(TILE(0,2),TILE(1,0),TILE(1,1),TILE(1,2))
};

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

static Engine_t game;
static GameSnapshot_t published;
static uint8_t frame_available = 0U;
static uint32_t sound_deadline = 0U;
static uint8_t sound_active = 0U;
static uint32_t rng_state = 0xA341316CUL;

static uint8_t random_piece(void)
{
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return (uint8_t)(rng_state % 7U);
}

static uint32_t gravity_ms(void)
{
    if (game.difficulty == DIFFICULTY_EASY) return 800U;
    if (game.difficulty == DIFFICULTY_HARD) return 250U;
    return 500U;
}

static void tone(uint16_t hz, uint32_t ms)
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

static void tone_poll(void)
{
    if (sound_active && (int32_t)(HAL_GetTick() - sound_deadline) >= 0) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, 0U);
        sound_active = 0U;
    }
}

static uint8_t occupied(uint16_t mask, uint8_t row, uint8_t col)
{
    return (uint8_t)((mask >> (row * 4U + col)) & 1U);
}

static uint8_t can_place(uint16_t mask, int8_t px, int8_t py)
{
    uint8_t r,c;
    for (r = 0; r < 4U; r++) for (c = 0; c < 4U; c++) {
        if (occupied(mask,r,c)) {
            int8_t x = (int8_t)(px + (int8_t)c);
            int8_t y = (int8_t)(py + (int8_t)r);
            if (x < 0 || x >= (int8_t)TETRIS_COLS ||
                y < 0 || y >= (int8_t)TETRIS_ROWS) return 0U;
            if (game.board[(uint8_t)y][(uint8_t)x]) return 0U;
        }
    }
    return 1U;
}

static uint16_t rotate_mask(uint16_t mask)
{
    uint16_t out = 0U;
    uint8_t r,c;
    for (r=0; r<4U; r++) for (c=0; c<4U; c++)
        if (occupied(mask,r,c)) out |= TILE(c,3U-r);
    return out;
}

static uint8_t try_move(int8_t dx, int8_t dy)
{
    int8_t x = (int8_t)(game.piece_x + dx);
    int8_t y = (int8_t)(game.piece_y + dy);
    if (!can_place(game.mask,x,y)) return 0U;
    game.piece_x = x;
    game.piece_y = y;
    return 1U;
}

static uint8_t try_rotate(void)
{
    uint16_t newmask = rotate_mask(game.mask);
    /* Simplified wall kick (not full SRS). */
    static const int8_t kicks[] = {0, -1, 1, -2, 2};
    uint8_t k;
    for(k=0; k<sizeof(kicks)/sizeof(kicks[0]); k++) {
        int8_t newx = (int8_t)(game.piece_x + kicks[k]);
        if(can_place(newmask,newx,game.piece_y)) {
            game.piece_x = newx;
            game.mask = newmask;
            return 1U;
        }
    }
    return 0U;
}

static void spawn_next(void)
{
    game.current_piece = game.next_piece;
    game.next_piece = random_piece();
    game.mask = piece_masks[game.current_piece];
    game.piece_x = 3;
    game.piece_y = 0;
    if (!can_place(game.mask,game.piece_x,game.piece_y)) {
        game.state = STATE_GAME_OVER;
        game.menu_index = 0U;
        tone(350U, 220U);
    }
}

static void start_game(void)
{
    memset(game.board,0,sizeof(game.board));
    game.score = 0U;
    game.lines = 0U;
    game.menu_index = 0U;
    game.state = STATE_PLAYING;
    game.last_fall_tick = HAL_GetTick();
    game.next_piece = random_piece();
    spawn_next();
}

static void lock_piece(void)
{
    uint8_t r,c,y,src,dst;
    uint8_t cleared = 0U;
    static const uint16_t score_table[5] = {0,100,300,500,800};
    for (r=0; r<4U; r++) for(c=0; c<4U; c++) {
        if (occupied(game.mask,r,c)) {
            int8_t x = (int8_t)(game.piece_x+c);
            int8_t yy = (int8_t)(game.piece_y+r);
            if (x>=0 && x<(int8_t)TETRIS_COLS &&
                yy>=0 && yy<(int8_t)TETRIS_ROWS)
                game.board[(uint8_t)yy][(uint8_t)x] = (uint8_t)(game.current_piece+1U);
        }
    }
    /* Compact all surviving rows from bottom to top. */
    dst = TETRIS_ROWS;
    for (src=TETRIS_ROWS; src>0U;) {
        uint8_t full = 1U;
        src--;
        for (c=0; c<TETRIS_COLS; c++) {
            if(game.board[src][c] == 0U) {full=0U;break;}
        }
        if(full) {cleared++;continue;}
        dst--;
        if(dst != src) memcpy(game.board[dst],game.board[src],TETRIS_COLS);
    }
    for(y=0; y<dst; y++) memset(game.board[y],0,TETRIS_COLS);
    if (cleared) {
        game.lines += cleared;
        game.score += (uint32_t)score_table[cleared] *
                      (uint32_t)(game.lines/10U + 1U);
        tone(1500U, 110U);
    }
    /* WIN wins priority over GAME_OVER if both would happen on this turn. */
    if(game.score >= TETRIS_WIN_SCORE) {
        game.state = STATE_WIN;
        game.menu_index = 0U;
        tone(2200U, 200U);
        return;
    }
    spawn_next();
}

static uint8_t handle_button(uint16_t raw)
{
    ButtonCode_t key = (ButtonCode_t)(raw & BUTTON_CODE_MASK);
    uint8_t repeated = ((raw & BUTTON_REPEAT_FLAG) != 0U);
    if (repeated && game.state != STATE_PLAYING) return 0U;

    switch(game.state) {
    case STATE_WELCOME:
        if (key == BTN_DROP || key == BTN_RIGHT) {
            game.state = STATE_MAIN_MENU;
            game.menu_index=0U; return 1U;
        }
        return 0U;
    case STATE_MAIN_MENU:
        if(key==BTN_ROTATE) {game.menu_index=(game.menu_index+1U)%2U;return 1U;}
        if(key==BTN_DOWN)   {game.menu_index=(game.menu_index+1U)%2U;return 1U;}
        if(key==BTN_PAUSE || key==BTN_LEFT) {game.state=STATE_WELCOME;return 1U;}
        if(key==BTN_DROP || key==BTN_RIGHT) {
            game.state = game.menu_index?STATE_HELP:STATE_DIFFICULTY;
            game.menu_index=0U; return 1U;
        }
        return 0U;
    case STATE_HELP:
        if(key==BTN_PAUSE || key==BTN_LEFT || key==BTN_DROP) {
            game.state=STATE_MAIN_MENU;game.menu_index=0U; return 1U;
        }
        return 0U;
    case STATE_DIFFICULTY:
        if(key==BTN_ROTATE) {game.menu_index=(game.menu_index+2U)%3U;return 1U;}
        if(key==BTN_DOWN) {game.menu_index=(game.menu_index+1U)%3U;return 1U;}
        if(key==BTN_PAUSE || key==BTN_LEFT) {
            game.state=STATE_MAIN_MENU;game.menu_index=0U;return 1U;
        }
        if(key==BTN_DROP || key==BTN_RIGHT) {
            game.difficulty=(Difficulty_t)game.menu_index;
            start_game();return 1U;
        }
        return 0U;
    case STATE_PLAYING:
        if(key==BTN_LEFT) return try_move(-1,0);
        if(key==BTN_RIGHT) return try_move(1,0);
        if(key==BTN_ROTATE && !repeated) {
            if (try_rotate()) {tone(900U,35U);return 1U;}
            return 0U;
        }
        if(key==BTN_DOWN) {
            if(!try_move(0,1)) lock_piece();
            return 1U;
        }
        if(key==BTN_DROP && !repeated) {
            while(try_move(0,1)) {} /* Maximum 20 rows: finite and nonblocking. */
            lock_piece();return 1U;
        }
        if(key==BTN_PAUSE && !repeated) {
            game.state=STATE_PAUSED;game.menu_index=0U;return 1U;
        }
        return 0U;
    case STATE_PAUSED:
        if(key==BTN_ROTATE) {game.menu_index=(game.menu_index+2U)%3U;return 1U;}
        if(key==BTN_DOWN) {game.menu_index=(game.menu_index+1U)%3U;return 1U;}
        if(key==BTN_PAUSE) {
            game.state=STATE_PLAYING;game.last_fall_tick=HAL_GetTick();return 1U;
        }
        if(key==BTN_DROP || key==BTN_RIGHT) {
            if(game.menu_index==0U) {
                game.state=STATE_PLAYING;game.last_fall_tick=HAL_GetTick();
            } else if(game.menu_index==1U) start_game();
            else {game.state=STATE_MAIN_MENU;game.menu_index=0U;}
            return 1U;
        }
        return 0U;
    case STATE_WIN:
    case STATE_GAME_OVER:
        if(key==BTN_ROTATE || key==BTN_DOWN) {
            game.menu_index^=1U;return 1U;
        }
        if(key==BTN_PAUSE) {
            game.state=STATE_MAIN_MENU;game.menu_index=0U;return 1U;
        }
        if(key==BTN_DROP || key==BTN_RIGHT) {
            if(game.menu_index==0U) start_game();
            else {game.state=STATE_MAIN_MENU;game.menu_index=0U;}
            return 1U;
        }
        return 0U;
    default: return 0U;
    }
}

static void publish_snapshot(void)
{
    GameSnapshot_t frame;
    uint8_t r,c;
    uint16_t token=1U;
    memset(&frame,0,sizeof(frame));
    memcpy(frame.cells,game.board,sizeof(game.board));
    if(game.state==STATE_PLAYING || game.state==STATE_PAUSED) {
        for(r=0;r<4U;r++) for(c=0;c<4U;c++) {
            if (occupied(game.mask,r,c)) {
                int8_t x=(int8_t)(game.piece_x+c);
                int8_t y=(int8_t)(game.piece_y+r);
                if(x>=0&&x<(int8_t)TETRIS_COLS &&
                   y>=0&&y<(int8_t)TETRIS_ROWS)
                    frame.cells[(uint8_t)y][(uint8_t)x]=game.current_piece+1U;
            }
        }
    }
    frame.score=game.score;
    frame.lines=game.lines;
    frame.level=(uint8_t)(game.lines/10U);
    frame.menu_index=game.menu_index;
    frame.state=game.state;
    frame.difficulty=game.difficulty;
    frame.next_piece=game.next_piece;
    /* Small bounded copy (~220 bytes) protected from DisplayTask read. */
    taskENTER_CRITICAL();
    published=frame;
    frame_available=1U;
    taskEXIT_CRITICAL();
    /* Queue capacity 1: if already full, the latest snapshot is still stored. */
    (void)osMessageQueuePut(xFrameQueueHandle,&token,0U,0U);
}

uint8_t app_game_copy_snapshot(GameSnapshot_t *out)
{
    uint8_t ready;
    if (out == NULL) return 0U;
    taskENTER_CRITICAL();
    ready=frame_available;
    if(ready) *out=published;
    taskEXIT_CRITICAL();
    return ready;
}

void task_game_run(void *argument)
{
    TickType_t last=xTaskGetTickCount();
    const TickType_t period=pdMS_TO_TICKS(TETRIS_GAME_PERIOD_MS);
    (void)argument;
    memset(&game,0,sizeof(game));
    game.state=STATE_WELCOME;
    game.difficulty=DIFFICULTY_NORMAL;
    rng_state ^= HAL_GetTick();
    (void)HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,0U);
    publish_snapshot();

    for (;;) {
        uint16_t raw;
        uint8_t changed=0U;
        uint8_t iter=0U;
        /* Cap events per cycle to preserve predictable scheduling. */
        while(iter++ < 16U &&
              osMessageQueueGet(xInputQueueHandle,&raw,NULL,0U)==osOK) {
            if(handle_button(raw)) changed=1U;
        }
        if(game.state==STATE_PLAYING) {
            uint32_t now=HAL_GetTick();
            if((uint32_t)(now-game.last_fall_tick)>=gravity_ms()) {
                game.last_fall_tick=now;
                if(!try_move(0,1)) lock_piece();
                changed=1U;
            }
        }
        tone_poll();
        if(changed) publish_snapshot();
        vTaskDelayUntil(&last,period);
    }
}
