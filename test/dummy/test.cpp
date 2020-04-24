#include "catch.hpp"

extern "C"
{
#include "dummy/dummy.h"

#include "dummy/time_config.h"
}

#define SEC_PER_MIN         60
#define MIN_TO_SEC(min)     ((min) * SEC_PER_MIN)

static const game_time_t DEFAULT_BASE_TIME = MIN_TO_SEC(5);

static void init_game_with_time_config(game_time_t base_time, game_time_t bonus_per_move = 0)
{
    struct time_config config;
    config.base_time = base_time;
    config.bonus_per_move = bonus_per_move;
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


SCENARIO("Time ended")
{
    GIVEN("Both players have 1s left")
    {
        init_game_with_time_config(1);

        WHEN("Game Started")
        {
            game_start();

            THEN("Time didn't end for any player")
            {
                REQUIRE(PLAYER_NONE == game_which_player_exceeded_time_first());
            }

            WHEN("Second elapsed for Player1")
            {
                current_player_second_elapsed();

                THEN("Time ended for Player1")
                {
                    REQUIRE(PLAYER_1 == game_which_player_exceeded_time_first());
                }

                WHEN("Player1 moved")
                {
                    game_current_player_moved();

                    THEN("Time ended for Player1")
                    {
                        REQUIRE(PLAYER_1 == game_which_player_exceeded_time_first());
                    }

                    WHEN("Second elapsed for Player2")
                    {
                        current_player_second_elapsed();

                        THEN("Time ended for Player1")
                        {
                            REQUIRE(PLAYER_1 == game_which_player_exceeded_time_first());
                        }
                    }
                }
            }

            WHEN("Second elapsed for Player2")
            {
                game_current_player_moved();
                current_player_second_elapsed();

                THEN("Time ended for Player2")
                {
                    REQUIRE(PLAYER_2 == game_which_player_exceeded_time_first());
                }

                WHEN("Player2 moved")
                {
                    game_current_player_moved();

                    THEN("Time ended for Player2")
                    {
                        REQUIRE(PLAYER_2 == game_which_player_exceeded_time_first());
                    }

                    WHEN("Second elapsed for Player1")
                    {
                        current_player_second_elapsed();

                        THEN("Time ended for Player2")
                        {
                            REQUIRE(PLAYER_2 == game_which_player_exceeded_time_first());
                        }
                    }
                }
            }
        }
    }
}

SCENARIO("Bonus time per move")
{
    GIVEN("Given Player have 5 min left and bonus time 30 s")
    {
        init_game_with_time_config(DEFAULT_BASE_TIME, 30);
        game_start();

        WHEN("When Player1 moves")
        {
            game_current_player_moved();

            THEN("Player1 have 5 min 30 s left")
            {
                game_time_t time_left = player_get_time_left(PLAYER_1);
                REQUIRE(time_left == DEFAULT_BASE_TIME + 30);
            }

            THEN("Player2 have 5 min left")
            {
                game_time_t time_left = player_get_time_left(PLAYER_2);
                REQUIRE(time_left == DEFAULT_BASE_TIME);
            }

            WHEN("When Player2 moves")
            {
                game_current_player_moved();

                THEN("Player1 have 5 min 30 s left")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_1);
                    REQUIRE(time_left == DEFAULT_BASE_TIME + 30);
                }

                THEN("Player2 have 5 min 30 s left")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_2);
                    REQUIRE(time_left == DEFAULT_BASE_TIME + 30);
                }
            }
        }
    }

    //Handle time exceeded
}


