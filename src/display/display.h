#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "display_driver.h"

#include "time_config.h"

void display_init(const struct display_driver *driver);
void display_update(game_time_t time_p1, game_time_t time_p2, enum game_state state, enum player_id who_exceeded_time);
