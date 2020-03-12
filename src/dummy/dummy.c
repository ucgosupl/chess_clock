#include "dummy.h"

#include "time_config.h"

typedef uint32_t game_time_t;
typedef uint32_t moves_cnt_t;

struct player
{
    game_time_t time_left;
    moves_cnt_t moves_cnt;
};

enum game_state
{
    GAME_STATE_START,
    GAME_STATE_PLAYER1,
    GAME_STATE_PLAYER2,
    GAME_STATE_PAUSE,
    GAME_STATE_REFEREE_INTERVENTION,
    GAME_STATE_FINAL,
};

struct game
{
    enum player_id player_to_move;
    enum player_id player_exceeded_time_first;
    enum game_state game_state;
};

void game_init(struct time_config *time)
{

}

void game_move_done(void)
{

}

void game_start(void)
{

}

void game_pause(void)
{

}

void game_referee_intervention(game_time_t time_p1, moves_cnt_t moves_p1, game_time_t time_p2, moves_cnt_t moves_p2)
{

}

void player_second_elapsed(enum player_id player)
{
    
}

