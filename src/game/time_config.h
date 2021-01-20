#pragma once

#include "game_types.h"

#include <stdint.h>
#include <stdbool.h>

#define SEC_PER_MIN         60
#define MIN_TO_SEC(min)     ((min) * SEC_PER_MIN)

struct bonus_time_after_x_moves
{
    game_time_t time_added;
    moves_cnt_t moves;
};

struct time_config
{
    game_time_t base_time;
    game_time_t bonus_per_move;

    game_time_t time_added;
    moves_cnt_t moves;
};
