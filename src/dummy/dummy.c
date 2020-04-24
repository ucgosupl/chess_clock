#include "dummy.h"

#include "time_config.h"
#include <string.h>

typedef uint32_t moves_cnt_t;

struct player
{
    game_time_t time_left;

    //moves_cnt_t moves_cnt;
};

enum game_state
{
    GAME_STATE_PAUSED,
    GAME_STATE_STARTED,

    // GAME_STATE_START,
    // GAME_STATE_PLAYER1,
    // GAME_STATE_PLAYER2,
    // GAME_STATE_PAUSE,
    // GAME_STATE_REFEREE_INTERVENTION,
    // GAME_STATE_FINAL,
};

struct game
{
    struct player player1;
    struct player player2;

    enum game_state state;
    enum player_id player_to_move;

    enum player_id player_exceeded_time_first;

    struct time_config current_time_config;
};

static struct game game_state;

static void time_config_for_player(struct player *player, game_time_t base_time);
static struct player * get_params_for_player(enum player_id player);
static void decrease_time_for_player(enum player_id player);
static void decrease_time_for_current_player(void);
static void update_player_exceeded_time(enum player_id player);
static void update_time_when_player_moved(enum player_id player);

void game_init(struct time_config *time)
{
    game_state.state = GAME_STATE_PAUSED;
    game_state.player_to_move = PLAYER_NONE;
    game_state.player_exceeded_time_first = PLAYER_NONE;

    time_config_for_player(&game_state.player1, time->base_time);
    time_config_for_player(&game_state.player2, time->base_time);

    memcpy(&game_state.current_time_config, time, sizeof(struct time_config));
}

void game_current_player_moved(void)
{
    switch(game_state.player_to_move)
    {
        case PLAYER_NONE:
            //illegal case
            break;
        
        case PLAYER_1:
            update_time_when_player_moved(PLAYER_1);
            game_state.player_to_move = PLAYER_2;
            break;

        case PLAYER_2:
            update_time_when_player_moved(PLAYER_2);
            game_state.player_to_move = PLAYER_1;
            break;
        
        default:
            //illegal case
            break;
    }
}

void game_start(void)
{
    game_state.state = GAME_STATE_STARTED;

    if (PLAYER_NONE == game_state.player_to_move)
    {
        game_state.player_to_move = PLAYER_1;
    }
}

void game_pause(void)
{
    game_state.state = GAME_STATE_PAUSED;
}

bool game_is_started(void)
{
    return GAME_STATE_STARTED == game_state.state;
}

void game_referee_intervention(game_time_t time_p1, moves_cnt_t moves_p1, game_time_t time_p2, moves_cnt_t moves_p2)
{

}

void current_player_second_elapsed(void)
{
    if (true == game_is_started())
    {
        decrease_time_for_current_player();
    }
}

game_time_t player_get_time_left(enum player_id player)
{
    switch (player)
    {
        case PLAYER_NONE:
            //illegal case
            return -1;

        case PLAYER_1:
            return game_state.player1.time_left;
        
        case PLAYER_2:
            return game_state.player2.time_left;
        
        default:
            //illegal case
            return -1;
    }
}

enum player_id game_player_to_move(void)
{
    return game_state.player_to_move;
}

enum player_id game_which_player_exceeded_time_first(void)
{
    return game_state.player_exceeded_time_first;
}

static void time_config_for_player(struct player *player, game_time_t base_time)
{
    player->time_left = base_time;
}

static struct player * get_params_for_player(enum player_id player)
{
    switch (player)
    {
        case PLAYER_1:
            return &game_state.player1;
        case PLAYER_2:
            return &game_state.player2;
        default:
            return NULL;
    }
}

static void decrease_time_for_player(enum player_id player)
{
    struct player *player_params = get_params_for_player(player);

    if (NULL != player_params)
    {
        player_params->time_left--;

        if (0 == player_params->time_left)
        {
            update_player_exceeded_time(player);
        }
    }
}

static void decrease_time_for_current_player(void)
{
    switch (game_state.player_to_move)
    {
    case PLAYER_NONE:
        //illegal case
        break;

    case PLAYER_1:
        decrease_time_for_player(PLAYER_1);
        break;

    case PLAYER_2:
        decrease_time_for_player(PLAYER_2);
        break;

    default:
        //illegal case
        break;
    }
}

static void update_player_exceeded_time(enum player_id player)
{
    if (PLAYER_NONE == game_state.player_exceeded_time_first)
    {
        game_state.player_exceeded_time_first = player;
    }
}

static void update_time_when_player_moved(enum player_id player)
{
    struct player *player_params = get_params_for_player(player);

    if (NULL != player_params)
    {
        if (0 < player_params->time_left)
        {
            player_params->time_left += game_state.current_time_config.bonus_per_move;
        }
    }
}
