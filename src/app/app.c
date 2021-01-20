#include "app.h"

#include "config/config.h"

enum app_state
{
    APP_STATE_UNKNOWN,
    APP_STATE_INIT,
    APP_STATE_CONFIG,
    APP_STATE_GAME,
    APP_STATE_REFEREE_INTERVENTION,
    APP_STATE_ERROR,
    APP_STATE_MAX,
};

static enum app_state state = APP_STATE_UNKNOWN;

static void app_state_init(void);
static void app_state_error(void);
static void app_state_config(void);
static void app_state_game(void);
static void app_state_referee_intervention(void);

void app_init(void)
{
    state = APP_STATE_INIT;
}

void app_error(void)
{
    state = APP_STATE_ERROR;
}

void app_update(void)
{
    //todo: handle current state
}

static void app_state_init(void)
{
    //proceed with init

    state = APP_STATE_CONFIG;

    //todo: handle error and set error state
}

static void app_state_error(void)
{
    //todo: print error msg
}

static void app_state_config(void)
{
    cfg_update();

    if (cfg_is_done())
    {
        enum time_mode_id mode = cfg_mode_get();
        struct time_config tcfg;
        cfg_fill(&tcfg);

        config_time(mode, &tcfg);
        game_init(&tcfg);

        state = APP_STATE_GAME;
    }
}

static void app_state_game(void)
{
    //todo: handle start, pause, intervention
}

static void app_state_referee_intervention(void)
{
    referee_update();

    if (referee_is_done())
    {
        game_time_t time_p1 = referee_time_p1_get();
        game_time_t time_p2 = referee_time_p2_get();
        moves_cnt_t moves = referee_moves_get();
        enum player_id next_to_move = referee_next_to_move_get();

        game_referee_intervention(time_p1, time_p2, moves, next_to_move);
    }
}
