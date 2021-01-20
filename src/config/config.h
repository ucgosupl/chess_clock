#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "time_config.h"

enum time_mode_id
{
    TIME_MODE_UNKNOWN,

    //Initial time only
    TIME_MODE_5MIN,
    TIME_MODE_15MIN,
    TIME_MODE_30MIN,
    TIME_MODE_CUSTOM_TIME,

    //Initial + bonus per move
    TIME_MODE_3MIN_BONUS_2S,
    TIME_MODE_5MIN_BONUS_3S,
    TIME_MODE_10MIN_BONUS_5S,
    TIME_MODE_30MIN_BONUS_30S,
    TIME_MODE_CUSTOM_TIME_BONUS,

    //Initial + time control
    TIME_MODE_90MIN_CONTROL_30M_60MIN,
    TIME_MODE_120MIN_CONTROL_40M_60MIN,
    TIME_MODE_CUSTOM_TIME_CONTROL,

    //Initial + bonus per move + time control
    TIME_MODE_75MIN_CONTROL_30M_30MIN_BONUS_30S,
    TIME_MODE_CUSTOM_TIME_CONTROL_BONUS,

    TIME_MODE_MAX,
};

const struct config * config_time(struct time_config * tcfg, enum time_mode_id mode);
