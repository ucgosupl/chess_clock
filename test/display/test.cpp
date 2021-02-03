#include "catch.hpp"

#include <cstring>

extern "C"
{
#include "display/display.h"

#include "time_config.h"
}

static const char * mock_p1_time = nullptr;
static const char * mock_p2_time = nullptr;

static void mock_reset()
{
    mock_p1_time = nullptr;
    mock_p2_time = nullptr;
}

static void mock_driver_update(const char *p1_time, const char *p2_time)
{
    mock_p1_time = p1_time;
    mock_p2_time = p2_time;
}


SCENARIO("Converting game time to string")
{
    GIVEN("Both player have 30 mins")
    {
        game_time_t time_p1 = MIN_TO_SEC(30);
        game_time_t time_p2 = MIN_TO_SEC(30);

        const struct display_driver mock_driver = {NULL, mock_driver_update, NULL, NULL};

        mock_reset();
        display_init(&mock_driver);

        WHEN("Display updated")
        {
            display_update(time_p1, time_p2, GAME_STARTED, NONE);

            THEN("Driver receives 0:30 for player 1")
            {
                REQUIRE(mock_p1_time != nullptr);
                REQUIRE(0 == strcmp(" 0:30", mock_p1_time));
            }

            THEN("Driver receives 0:30 for player 2")
            {
                REQUIRE(mock_p2_time != nullptr);
                REQUIRE(0 == strcmp(" 0:30", mock_p2_time));
            }
        }
    }
}

//when 30 min set -> 0:30 displayed
//when 20 min set -> 0:20 displayed
//when 19.59 set -> 19.59 displayed
//when 15 min set -> 15.00 displayed