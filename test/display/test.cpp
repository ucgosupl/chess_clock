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

    GIVEN("Both player have 60 mins")
    {
        game_time_t time_p1 = MIN_TO_SEC(60);
        game_time_t time_p2 = MIN_TO_SEC(60);

        const struct display_driver mock_driver = {NULL, mock_driver_update, NULL, NULL};

        mock_reset();
        display_init(&mock_driver);

        WHEN("Display updated")
        {
            display_update(time_p1, time_p2, GAME_STARTED, NONE);

            THEN("Driver receives 1:00 for player 1")
            {
                REQUIRE(mock_p1_time != nullptr);
                REQUIRE(0 == strcmp(" 1:00", mock_p1_time));
            }

            THEN("Driver receives 1:00 for player 2")
            {
                REQUIRE(mock_p2_time != nullptr);
                REQUIRE(0 == strcmp(" 1:00", mock_p2_time));
            }
        }
    }

    GIVEN("Both player have 10h")
    {
        game_time_t time_p1 = MIN_TO_SEC(60 * 10);
        game_time_t time_p2 = MIN_TO_SEC(60 * 10);

        const struct display_driver mock_driver = {NULL, mock_driver_update, NULL, NULL};

        mock_reset();
        display_init(&mock_driver);

        WHEN("Display updated")
        {
            display_update(time_p1, time_p2, GAME_STARTED, NONE);

            THEN("Driver receives 10:00 for player 1")
            {
                REQUIRE(mock_p1_time != nullptr);
                REQUIRE(0 == strcmp("10:00", mock_p1_time));
            }

            THEN("Driver receives 10:00 for player 2")
            {
                REQUIRE(mock_p2_time != nullptr);
                REQUIRE(0 == strcmp("10:00", mock_p2_time));
            }
        }
    }
}

SCENARIO("Change format when less than 20 min left")
{
    GIVEN("Both players have 20:00 min left")
    {
        game_time_t time_p1 = MIN_TO_SEC(20);
        game_time_t time_p2 = MIN_TO_SEC(20);

        const struct display_driver mock_driver = {NULL, mock_driver_update, NULL, NULL};

        mock_reset();
        display_init(&mock_driver);

        WHEN("Display updated")
        {
            display_update(time_p1, time_p2, GAME_STARTED, NONE);

            THEN("Driver receives 0:20 for player 1")
            {
                assert(mock_p1_time != nullptr);
                REQUIRE(0 == strcmp(" 0:20", mock_p1_time));
            }

            THEN("Driver receives 0:20 for player 2")
            {
                assert(mock_p2_time != nullptr);
                REQUIRE(0 == strcmp(" 0:20", mock_p2_time));
            }
        }

        WHEN("Second elapsed for player 1")
        {
            time_p1 -= 1;

            WHEN("Display updated")
            {
                display_update(time_p1, time_p2, GAME_STARTED, NONE);

                THEN("Driver receives 19.59 for player 1")
                {
                    assert(mock_p1_time != nullptr);
                    REQUIRE(0 == strcmp("19.59", mock_p1_time));
                }

                THEN("Driver receives 0:20 for player 2")
                {
                    assert(mock_p2_time != nullptr);
                    REQUIRE(0 == strcmp(" 0:20", mock_p2_time));
                }
            }

            WHEN("Second elapsed for player 2")
            {
                time_p2 -= 1;

                WHEN("Display updated")
                {
                    display_update(time_p1, time_p2, GAME_STARTED, NONE);

                    THEN("Driver receives 19.59 for player 1")
                    {
                        assert(mock_p1_time != nullptr);
                        REQUIRE(0 == strcmp("19.59", mock_p1_time));
                    }

                    THEN("Driver receives 19.59 for player 2")
                    {
                        assert(mock_p2_time != nullptr);
                        REQUIRE(0 == strcmp("19.59", mock_p2_time));
                    }
                }
            }
        }
    }
}

SCENARIO("Display when less than 10 min")
{
    GIVEN("Both players have 9:59 min left")
    {
        game_time_t time_p1 = MIN_TO_SEC(10) - 1;
        game_time_t time_p2 = MIN_TO_SEC(10) - 1;

        const struct display_driver mock_driver = {NULL, mock_driver_update, NULL, NULL};

        mock_reset();
        display_init(&mock_driver);

        WHEN("Display updated")
        {
            display_update(time_p1, time_p2, GAME_STARTED, NONE);

            THEN("Driver receives 9.59 for player 1")
            {
                assert(mock_p1_time != nullptr);
                REQUIRE(0 == strcmp(" 9.59", mock_p1_time));
            }

            THEN("Driver receives 9.59 for player 2")
            {
                assert(mock_p2_time != nullptr);
                REQUIRE(0 == strcmp(" 9.59", mock_p2_time));
            }
        }
    }
}

SCENARIO("Display when less than 10 sec")
{
    GIVEN("Both players have 0.09 left")
    {
        game_time_t time_p1 = 9;
        game_time_t time_p2 = 9;

        const struct display_driver mock_driver = {NULL, mock_driver_update, NULL, NULL};

        mock_reset();
        display_init(&mock_driver);

        WHEN("Display updated")
        {
            display_update(time_p1, time_p2, GAME_STARTED, NONE);

            THEN("Driver receives 0.09 for player 1")
            {
                assert(mock_p1_time != nullptr);
                REQUIRE(0 == strcmp(" 0.09", mock_p1_time));
            }

            THEN("Driver receives 0.09 for player 2")
            {
                assert(mock_p2_time != nullptr);
                REQUIRE(0 == strcmp(" 0.09", mock_p2_time));
            }
        }
    }
}
