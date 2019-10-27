cmake_minimum_required(VERSION 3.13.4)
project(tests LANGUAGES C)

include_directories(./ ../src/)
link_directories(../src/lexer/build)
# set(CMAKE_PREFIX_PATH "../src/lexer/build/")

add_executable(tests main.c dfa_tests.c)
# find_package(lexer NAMES liblexer PATHS ../src/lexer/build/)
# find_package(LEXER_LIB NAMES lexer)
target_link_libraries(tests lexer)

find_package(cmocka)
target_link_libraries(tests cmocka)
