#pragma once

#include <stdint.h>
#include <stdbool.h>

#define TIME_TEXT_SIZE      5

enum game_state
{
    GAME_STOPPED,
    GAME_STARTED,
    GAME_PAUSED,
};

enum player_id
{
    NONE,
    PLAYER1,
    PLAYER2,
};

typedef void (*init_fun_t)(void);
typedef void (*update_time_fun_t)(const char *p1_time, const char *p2_time);
typedef void (*update_state_fun_t)(enum game_state state);
typedef void (*update_excedeed_fun_t)(enum player_id who_exceeded_time);

struct display_driver
{
    init_fun_t init;
    update_time_fun_t update_time;
    update_state_fun_t update_state;
    update_excedeed_fun_t update_exceeded;
};
