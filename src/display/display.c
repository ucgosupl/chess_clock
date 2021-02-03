#include "display.h"

#include <stdlib.h>
#include <assert.h>

struct display_params
{
    const struct display_driver *driver;

    char p1_time[TIME_TEXT_SIZE + 1];
    char p2_time[TIME_TEXT_SIZE + 1];

};

static struct display_params params;

static struct display_driver *drv = NULL;

static void gametime_to_char(game_time_t time, char *buf);
static void convert_hour_min(game_time_t time, char *buf);
static void convert_min_sec(game_time_t time, char *buf);

static void itoa_with_blanks(uint32_t num, char *buf);
static void itoa_with_zeros(uint32_t num, char *buf);

void display_init(const struct display_driver *driver)
{
    params.driver = driver;
}

void display_update(game_time_t time_p1, game_time_t time_p2, enum game_state state, enum player_id who_exceeded_time)
{
    assert(params.driver != NULL);

    gametime_to_char(time_p1, params.p1_time);
    gametime_to_char(time_p2, params.p2_time);

    params.driver->update_time(params.p1_time, params.p2_time);
}

static void gametime_to_char(game_time_t time, char *buf)
{
    if ((time / 60) < 20)
    {
        convert_min_sec(time, buf);
    }
    else
    {
        convert_hour_min(time, buf);
    }
}

static void convert_hour_min(game_time_t time, char *buf)
{
        uint32_t hour = time / (60 * 60);
        uint32_t min = time / 60 - hour * 60;

        assert(hour < 100);
        assert(min < 60);

        itoa_with_blanks(hour, buf);
        buf[2] = ':';
        itoa_with_zeros(min, &buf[3]);
        buf[5] = '\0';
}

static void convert_min_sec(game_time_t time, char *buf)
{
        uint32_t min = time / 60;
        uint32_t sec = time % 60;

        itoa_with_blanks(min, buf);
        buf[2] = '.';
        itoa_with_zeros(sec, &buf[3]);
        buf[5] = '\0';
}

static void itoa_with_blanks(uint32_t num, char *buf)
{
    if (num < 10)
    {
        buf[0] = ' ';
        itoa(num, &buf[1], 10);
    }
    else
    {
        itoa(num, buf, 10);
    }
}

static void itoa_with_zeros(uint32_t num, char *buf)
{
    if (num < 10)
    {
        buf[0] = '0';
        itoa(num, &buf[1], 10);
    }
    else
    {
        itoa(num, buf, 10);
    }
}


