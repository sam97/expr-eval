cmake_minimum_required(VERSION 3.13.4)
project(lexer LANGUAGES C)

include_directories(./ ../)
add_library(lexer STATIC dfa.c)
