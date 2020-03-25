#include "catch.hpp"

extern "C"
{
#include "dummy/dummy.h"

#include "dummy/time_config.h"
}

#define SEC_PER_MIN         60
#define MIN_TO_SEC(min)     ((min) * SEC_PER_MIN)

static const game_time_t DEFAULT_BASE_TIME = MIN_TO_SEC(5);

static void init_game_with_time_config(game_time_t base_time)
{
    struct time_config config;
    config.base_time = base_time;
    game_init(&config);
}

static void init_game_with_defaults()
{
    init_game_with_time_config(DEFAULT_BASE_TIME);
}

SCENARIO("Game initialization")
{
    GIVEN("Time configured to 5 minutes")
    {
        const game_time_t time_expected = MIN_TO_SEC(5);

        WHEN("Game is initialized")
        {
            init_game_with_time_config(time_expected);

            THEN("Player1 have 5 minutes left")
            {
                game_time_t p1_time_left = player_get_time_left(PLAYER_1);

                REQUIRE(p1_time_left == time_expected);
            }

            THEN("Player2 have 5 minutes left")
            {
                game_time_t p2_time_left = player_get_time_left(PLAYER_2);

                REQUIRE(p2_time_left == time_expected);
            }
        }
    }

    GIVEN("Time configured to 15 minutes")
    {
        const game_time_t time_expected = MIN_TO_SEC(15);

        WHEN("Game is initialized")
        {
            init_game_with_time_config(time_expected);

            THEN("Player1 have 15 minutes left")
            {
                game_time_t p1_time_left = player_get_time_left(PLAYER_1);

                REQUIRE(p1_time_left == time_expected);
            }

            THEN("Player2 have 15 minutes left")
            {
                game_time_t p2_time_left = player_get_time_left(PLAYER_2);

                REQUIRE(p2_time_left == time_expected);
            }
        }
    }
}

SCENARIO("Game starting")
{
    WHEN("Game is initialized")
    {
        init_game_with_defaults();

        THEN("Game is not started")
        {
            REQUIRE(false == game_is_started());
        }

        WHEN("Game started")
        {
            game_start();

            THEN("Game is started")
            {
                REQUIRE(true == game_is_started());
            }

            WHEN("Game is paused")
            {
                game_pause();

                THEN("Game is not started")
                {
                    REQUIRE(false == game_is_started());
                }

                WHEN("Game started again")
                {
                    game_start();

                    THEN("Game is started")
                    {
                        REQUIRE(true == game_is_started());
                    }
                }
            }
        }
    }
}

SCENARIO("Switching players to move")
{
    WHEN("Game is not started")
    {
        init_game_with_defaults();

        THEN("No player to move")
        {
            REQUIRE(PLAYER_NONE == game_player_to_move());
        }

        WHEN("Game started")
        {
            game_start();

            THEN("Player1 to move")
            {
                REQUIRE(PLAYER_1 == game_player_to_move());
            }

            WHEN("Player1 moved")
            {
                game_current_player_moved();

                THEN("Player2 to move")
                {
                    REQUIRE(PLAYER_2 == game_player_to_move());
                }

                WHEN("Player2 moved")
                {
                    game_current_player_moved();

                    THEN("Player1 to move")
                    {
                        REQUIRE(PLAYER_1 == game_player_to_move());
                    }
                }

                WHEN("Game paused")
                {
                    game_pause();

                    WHEN("Game is resumed")
                    {
                        game_start();

                        THEN("Player2 to move")
                        {
                            REQUIRE(PLAYER_2 == game_player_to_move());
                        }
                    }
                }
            }
        }
    }
}

SCENARIO("Second elapsed")
{
    GIVEN("After init - game not started")
    {
        init_game_with_time_config(DEFAULT_BASE_TIME);
    
        WHEN("Second elapsed for current player")
        {
            current_player_second_elapsed();

            THEN("Player1 time not decreased")
            {
                game_time_t p1_time_left = player_get_time_left(PLAYER_1);

                REQUIRE(DEFAULT_BASE_TIME == p1_time_left);
            }

            THEN("Player2 time not decreased")
            {
                game_time_t p2_time_left = player_get_time_left(PLAYER_2);

                REQUIRE(DEFAULT_BASE_TIME == p2_time_left);
            }
        }

        WHEN("Game started")
        {
            game_start();

            WHEN("Second elapsed for current player")
            {
                current_player_second_elapsed();

                THEN("Player1 time decreased")
                {
                    const game_time_t time_expected = DEFAULT_BASE_TIME - 1;
                    game_time_t p1_time_left = player_get_time_left(PLAYER_1);

                    REQUIRE(time_expected == p1_time_left);
                }

                THEN("Player2 time not decreased")
                {
                    game_time_t p2_time_left = player_get_time_left(PLAYER_2);

                    REQUIRE(DEFAULT_BASE_TIME == p2_time_left);
                }
            }

            WHEN("Game is paused")
            {
                game_pause();

                WHEN("Second elapsed for current player")
                {
                    current_player_second_elapsed();

                    THEN("Player1 time not decreased")
                    {
                        game_time_t p1_time_left = player_get_time_left(PLAYER_1);

                        REQUIRE(DEFAULT_BASE_TIME == p1_time_left);
                    }
                }
            }

            WHEN("Player moved")
            {
                game_current_player_moved();

                WHEN("Second elapsed for current player")
                {
                    current_player_second_elapsed();

                    THEN("Player1 time not decreased")
                    {
                        game_time_t p1_time_left = player_get_time_left(PLAYER_1);

                        REQUIRE(DEFAULT_BASE_TIME == p1_time_left);
                    }

                    THEN("Player2 time decreased")
                    {
                        const game_time_t time_expected = DEFAULT_BASE_TIME - 1;
                        game_time_t p2_time_left = player_get_time_left(PLAYER_2);

                        REQUIRE(time_expected == p2_time_left);
                    }
                }
            }
        }
    }
}


