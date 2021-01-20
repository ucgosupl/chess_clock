#include "catch.hpp"

extern "C"
{
#include "dummy/dummy.h"

#include "dummy/time_config.h"
}

#define SEC_PER_MIN         60
#define MIN_TO_SEC(min)     ((min) * SEC_PER_MIN)

static const game_time_t DEFAULT_BASE_TIME = MIN_TO_SEC(5);
static const game_time_t DEFAULT_TIME_ADDED = MIN_TO_SEC(3);

static void init_game_with_time_config(game_time_t base_time,
                                       game_time_t bonus_per_move,
                                       moves_cnt_t moves,
                                       game_time_t bonus_time)
{
    struct time_config config;
    config.base_time = base_time;
    config.bonus_per_move = bonus_per_move;
    config.bonus_time.moves = moves;
    config.bonus_time.time_added = bonus_time;
    game_init(&config);
}

static void init_game_with_base_time(game_time_t base_time)
{
    init_game_with_time_config(base_time, 0, 0, 0);
}

static void init_game_with_bonus_per_move(game_time_t base_time,
                                          game_time_t bonus_per_move)
{
    init_game_with_time_config(base_time, bonus_per_move, 0, 0);
}

static void init_game_with_time_control(game_time_t base_time,
                                        moves_cnt_t moves,
                                        game_time_t bonus_time)
{
    init_game_with_time_config(base_time, 0, moves, bonus_time);
}

static void init_game_with_time_control_and_bonus_per_move(game_time_t base_time,
                                                           game_time_t bonus_per_move,
                                                           moves_cnt_t moves,
                                                           game_time_t bonus_time)
{
    init_game_with_time_config(base_time, bonus_per_move, moves, bonus_time);
}

static void init_game_with_defaults()
{
    init_game_with_base_time(DEFAULT_BASE_TIME);
}

static void make_n_moves_for_both_players(moves_cnt_t n)
{
    moves_cnt_t i;

    for (i = 0; i < n; i++)
    {
        game_current_player_moved();
        game_current_player_moved();
    }
}

static void time_elapsed(game_time_t period)
{
    game_time_t i;

    for (i = 0; i < period; i++)
    {
        current_player_second_elapsed();
    }
}

SCENARIO("Game initialization")
{
    GIVEN("Time configured to 5 minutes")
    {
        const game_time_t time_expected = MIN_TO_SEC(5);

        WHEN("Game is initialized")
        {
            init_game_with_base_time(time_expected);

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
            init_game_with_base_time(time_expected);

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

                WHEN("Game resumed")
                {
                    game_resume();

                    THEN("Game is started")
                    {
                        REQUIRE(true == game_is_started());
                    }
                }
            }
        }
    }
}

SCENARIO("Game resume doesn't switch player")
{
    GIVEN("Game is initialized")
    {
        init_game_with_defaults();
        game_start();

        WHEN("Game is paused after player 1 moved")
        {
            game_current_player_moved();
            game_pause();

            THEN("Player 2 is next to move")
            {
                    REQUIRE(PLAYER_2 == game_player_to_move());
            }

            WHEN("Game is resumed")
            {
                THEN("Player 2 is next to move")
                {
                    REQUIRE(PLAYER_2 == game_player_to_move());
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
        init_game_with_base_time(DEFAULT_BASE_TIME);
    
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
        init_game_with_base_time(1);

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
    GIVEN("Players have 5 min left and bonus time 30 s")
    {
        init_game_with_bonus_per_move(DEFAULT_BASE_TIME, 30);

        WHEN("Game starts")
        {
            game_start();

            THEN("Player 1 has bonus time for first move")
            {
                game_time_t time_left = player_get_time_left(PLAYER_1);
                REQUIRE(time_left == DEFAULT_BASE_TIME + 30);
            }

            THEN("Player 2 has bonus time for first move")
            {
                game_time_t time_left = player_get_time_left(PLAYER_2);
                REQUIRE(time_left == DEFAULT_BASE_TIME + 30);
            }

            WHEN("Player1 moves")
            {
                game_current_player_moved();

                THEN("Player1 have 5 min + 2x 30 s left")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_1);
                    REQUIRE(time_left == DEFAULT_BASE_TIME + 2*30);
                }

                THEN("Player2 have 5 min + 30 s left")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_2);
                    REQUIRE(time_left == DEFAULT_BASE_TIME + 30);
                }

                WHEN("When Player2 moves")
                {
                    game_current_player_moved();

                    THEN("Player1 have 5 min + 2x 30 s left")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_1);
                        REQUIRE(time_left == DEFAULT_BASE_TIME + 2 * 30);
                    }

                    THEN("Player2 have 5 min + 2x 30 s left")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_2);
                        REQUIRE(time_left == DEFAULT_BASE_TIME + 2 * 30);
                    }
                }
            }
        }
    }
}

SCENARIO("Time exceeded with bonus time per move - Player 1")
{
    GIVEN("Players have 1 s left and bonus time 30 s - Players 1 turn")
    {
        init_game_with_bonus_per_move(1, 30);
        game_start();

        WHEN("30 sec elapsed")
        {
            time_elapsed(30);

            WHEN("Player 1 moved")
            {
                game_current_player_moved();

                THEN("Player 1 time left is 1 + 30s")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_1);
                    REQUIRE(time_left == 30 + 1);
                }
            }

            WHEN("Second elapsed")
            {
                current_player_second_elapsed();

                WHEN("Player 1 moved")
                {
                    game_current_player_moved();

                    THEN("Player 1 time left is 0")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_1);
                        REQUIRE(time_left == 0);
                    }
                }
            }
        }
    }
}

SCENARIO("Time exceeded with bonus time per move - Player 2")
{
    GIVEN("Players have 1 s left and bonus time 30 s - Players 2 turn")
    {
        init_game_with_bonus_per_move(1, 30);
        game_start();
        game_current_player_moved();

        WHEN("30 sec elapsed")
        {
            time_elapsed(30);

            WHEN("Player 2 moved")
            {
                game_current_player_moved();

                THEN("Player 2 time left is 1 + 30s")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_2);
                    REQUIRE(time_left == 30 + 1);
                }
            }

            WHEN("Second elapsed")
            {
                current_player_second_elapsed();

                WHEN("Player 2 moved")
                {
                    game_current_player_moved();

                    THEN("Player 2 time left is 0")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_2);
                        REQUIRE(time_left == 0);
                    }
                }
            }
        }
    }
}

SCENARIO("Time control")
{
    GIVEN("Rate is 5 min for 5 moves + 3 min")
    {
        init_game_with_time_control(DEFAULT_BASE_TIME, 5, DEFAULT_TIME_ADDED);
        game_start();

        WHEN("One move before control")
        {
            make_n_moves_for_both_players(4);

            THEN("Bonus time not added for player 1")
            {
                game_time_t time_left = player_get_time_left(PLAYER_1);
                REQUIRE(time_left == DEFAULT_BASE_TIME);
            }

            THEN("Bonus time not added for player 2")
            {
                game_time_t time_left = player_get_time_left(PLAYER_2);
                REQUIRE(time_left == DEFAULT_BASE_TIME);
            }

            WHEN("Player 1 moved")
            {
                game_current_player_moved();

                THEN("Bonus time added for player 1")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_1);
                    REQUIRE(time_left == DEFAULT_BASE_TIME + DEFAULT_TIME_ADDED);
                }

                THEN("Bonus time not added for player 2")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_2);
                    REQUIRE(time_left == DEFAULT_BASE_TIME);
                }

                WHEN("Player 2 moved")
                {
                    game_current_player_moved();

                    THEN("Bonus time added for player 2")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_2);
                        REQUIRE(time_left == DEFAULT_BASE_TIME + DEFAULT_TIME_ADDED);
                    }
                }
            }
                
        }   
    }
}

SCENARIO("Time control after pause")
{
    GIVEN("Rate is 5 min for 5 moves + 3 min")
    {
        init_game_with_time_control(DEFAULT_BASE_TIME, 5, DEFAULT_TIME_ADDED);
        game_start();

        WHEN("One move before control")
        {
            make_n_moves_for_both_players(4);

            WHEN("Game paused and resumed")
            {
                game_pause();
                game_resume();

                WHEN("Player 1 moved")
                {
                    game_current_player_moved();

                    THEN("Bonus time added for player 1")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_1);
                        REQUIRE(time_left == DEFAULT_BASE_TIME + DEFAULT_TIME_ADDED);
                    }

                    WHEN("Player 2 moved")
                    {
                        game_current_player_moved();

                        THEN("Bonus time added for player 2")
                        {
                            game_time_t time_left = player_get_time_left(PLAYER_2);
                            REQUIRE(time_left == DEFAULT_BASE_TIME + DEFAULT_TIME_ADDED);
                        }
                    }
                }
            }
        }
    }
}

SCENARIO("Referee intervention on Player 1 turn")
{
    GIVEN("Game is started")
    {
        init_game_with_time_control(DEFAULT_BASE_TIME, 5, DEFAULT_TIME_ADDED);
        game_start();

        GIVEN("Game is paused")
        {
            game_pause();

            WHEN("Referee intervention")
            {
                game_time_t p1time = 55;
                game_time_t p2time = 120;
                moves_cnt_t moves = 4;

                game_referee_intervention(p1time, p2time, moves, PLAYER_1);
                game_resume();

                THEN("Time for Player 1 matches referee intervention")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_1);
                    REQUIRE(time_left == p1time);
                }

                THEN("Time for Player 2 matches referee intervention")
                {
                    game_time_t time_left = player_get_time_left(PLAYER_2);
                    REQUIRE(time_left == p2time);
                }

                WHEN("Player 1 reached time control")
                {
                    game_current_player_moved();

                    THEN("Bonus time added for player 1")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_1);
                        REQUIRE(time_left == p1time + DEFAULT_TIME_ADDED);
                    }

                    THEN("Bonus time not added for player 2")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_2);
                        REQUIRE(time_left == p2time);
                    }

                    WHEN("Player 2 moved")
                    {
                        game_current_player_moved();

                        THEN("Bonus time added for player 2")
                        {
                            game_time_t time_left = player_get_time_left(PLAYER_2);
                            REQUIRE(time_left == p2time + DEFAULT_TIME_ADDED);
                        }
                    }
                }
            }
        }
    }
}

SCENARIO("Referee intervention on Player 2 turn")
{
    GIVEN("Game is started")
    {
        init_game_with_time_control(DEFAULT_BASE_TIME, 5, DEFAULT_TIME_ADDED);
        game_start();

        GIVEN("Game is paused")
        {
            game_pause();

            WHEN("Referee intervention")
            {
                game_time_t p1time = 55;
                game_time_t p2time = 120;
                moves_cnt_t moves = 4;

                game_referee_intervention(p1time, p2time, moves, PLAYER_2);
                game_resume();

                WHEN("Player 1 reached time control")
                {
                    game_current_player_moved();
                    game_current_player_moved();

                    THEN("Bonus time added for player 1")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_1);
                        REQUIRE(time_left == p1time + DEFAULT_TIME_ADDED);
                    }

                    THEN("Bonus time not added for player 2")
                    {
                        game_time_t time_left = player_get_time_left(PLAYER_2);
                        REQUIRE(time_left == p2time);
                    }

                    WHEN("Player 2 moved")
                    {
                        game_current_player_moved();

                        THEN("Bonus time added for player 2")
                        {
                            game_time_t time_left = player_get_time_left(PLAYER_2);
                            REQUIRE(time_left == p2time + DEFAULT_TIME_ADDED);
                        }
                    }
                }
            }
        }
    }
}
