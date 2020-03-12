cmake_minimum_required(VERSION 3.10)

project(template_tests)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g")
set(CMAKE_C_FLAGS_DEBUG "-Og -g")

set(INCLUDE_DIRS
	../../src
    ../catch2/single_include/catch2
    ../trompeloeil/include
	../mingw-std-threads
)

set(CPP_SRCS

)

set(C_SRCS
	../../src/cbuf.c
)

set(CPP_TEST_SRCS
	main.cpp
	test.cpp
)

set(C_TEST_SRCS

)

set(GLOBAL_DEFINES

)

set(LINKED_LIBS

)

link_directories(

)

include_directories(${INCLUDE_DIRS})
add_definitions(${GLOBAL_DEFINES})

#include(cppcheck.cmake)

# generate unit tests executable
add_executable(${CMAKE_PROJECT_NAME} ${CPP_SRCS} ${C_SRCS} ${CPP_TEST_SRCS} ${C_TEST_SRCS})

link_libraries(${CMAKE_PROJECT_NAME} ${LINKED_LIBS})
