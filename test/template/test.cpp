#include "catch.hpp"

SCENARIO("Initial failing test")
{
    GIVEN("asdgh")
    {
        WHEN("asdgh")
        {
            THEN("aaaa")
            {
                REQUIRE(false);
            }

            THEN("aaa")
            {
                REQUIRE(false);
            }
        }
    }
}

//buffer empty after init
//buffer not empty after adding item
//receive the same element that was added earlier
//overflow
//
