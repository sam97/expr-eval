cmake_minimum_required(VERSION 3.13.4)
project(expr-eval C)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -pedantic)
endif()

add_subdirectory(src/lexer)
add_subdirectory(tests)