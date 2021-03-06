#pragma once

#include "time_config.h"

#include <stdint.h>
#include <stdbool.h>

enum player_id
{
    PLAYER_NONE,
    PLAYER_1,
    PLAYER_2,
};

void game_init(struct time_config *time);

void game_current_player_moved(void);

void game_start(void);
void game_pause(void);
void game_resume(void);
bool game_is_started(void);

void game_referee_intervention(game_time_t time_p1,
                               game_time_t time_p2,
                               moves_cnt_t moves,
                               enum player_id next_to_move);

void game_second_elapsed_for_current_player(void);
// how to count seconds on hardware timers
// hw interface will need info that player made a move

game_time_t game_time_left_for_player(enum player_id player);

enum player_id game_player_to_move(void);
enum player_id game_which_player_exceeded_time_first(void);

