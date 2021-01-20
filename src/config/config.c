#include "config.h"

#include "game/time_config.h"

#define CUSTOM      -1
#define INVALID     -2

struct tcfg_record 
{
    enum time_mode_id mode;
    struct time_config config;
};

static const struct tcfg_record configurations[] = {
    {TIME_MODE_UNKNOWN, {INVALID, INVALID, INVALID, INVALID}},

    {TIME_MODE_5MIN, {MIN_TO_SEC(5), 0, 0, 0}},
    {TIME_MODE_15MIN, {MIN_TO_SEC(15), 0, 0, 0}},
    {TIME_MODE_30MIN, {MIN_TO_SEC(30), 0, 0, 0}},
    {TIME_MODE_CUSTOM_TIME, {CUSTOM, 0, 0, 0}},

    {TIME_MODE_3MIN_BONUS_2S, {MIN_TO_SEC(3), 2, 0, 0}},
    {TIME_MODE_5MIN_BONUS_3S, {MIN_TO_SEC(5), 3, 0, 0}},
    {TIME_MODE_10MIN_BONUS_5S, {MIN_TO_SEC(10), 5, 0, 0}},
    {TIME_MODE_30MIN_BONUS_30S, {MIN_TO_SEC(30), 30, 0, 0}},
    {TIME_MODE_CUSTOM_TIME_BONUS, {CUSTOM, CUSTOM, 0, 0}},

    {TIME_MODE_90MIN_CONTROL_30M_60MIN, {MIN_TO_SEC(90), 0, 30, MIN_TO_SEC(90)}},
    {TIME_MODE_120MIN_CONTROL_40M_60MIN, {MIN_TO_SEC(120), 0, 40, MIN_TO_SEC(90)}},
    {TIME_MODE_CUSTOM_TIME_CONTROL, {CUSTOM, 0, CUSTOM, CUSTOM}},

    {TIME_MODE_75MIN_CONTROL_30M_30MIN_BONUS_30S, {MIN_TO_SEC(75), 30, 30, MIN_TO_SEC(30)}},
    {TIME_MODE_CUSTOM_TIME_CONTROL_BONUS, {CUSTOM, CUSTOM, CUSTOM, CUSTOM}},
};

const struct config * config_time(struct time_config * tcfg, enum time_mode_id mode)
{
    const struct tcfg_record *record = &configurations[mode_to_idx(mode)];

    if (record->mode == TIME_MODE_UNKNOWN)
    {
        return NULL;
    }

    if (record->config.base_time != CUSTOM)
    {
        tcfg->base_time = record->config.base_time;
    }

    if (record->config.bonus_per_move != CUSTOM)
    {
        tcfg->bonus_per_move = record->config.bonus_per_move;
    }

    if (record->config.time_added != CUSTOM)
    {
        tcfg->time_added = record->config.time_added;
    }

    if (record->config.moves != CUSTOM)
    {
        tcfg->moves = record->config.moves;
    }

    return tcfg;
}

static size_t mode_to_idx(enum time_mode_id mode)
{
    if (mode <= 0 || mode >= TIME_MODE_MAX)
    {
        return TIME_MODE_UNKNOWN;
    }

    return (size_t)mode;
}
