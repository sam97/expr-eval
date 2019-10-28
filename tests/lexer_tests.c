#include "lexer_tests.h"
#include "lexer/lexer.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include <string.h>

static void test_lexer_good(char *source_code);
void test_lexer_next_good(Lex lexer, TokenType exp_type, char *exp_lexeme);

void test_lexer_init(void **state) {
    test_lexer_good("1 + 2");
    test_lexer_good("5 * 6");
    test_lexer_good("asdfasdf");
    test_lexer_good("1/0");
    test_lexer_good("");

    Lex lexer = lex_init(NULL);
    assert_null(lexer);
    lex_dispose(lexer);
}

void test_lexer_next(void **state) {
    Lex lexer = lex_init("1 + 2");

    assert_non_null(lexer);
    test_lexer_next_good(lexer, T_INT, "1");
    test_lexer_next_good(lexer, T_ADD, " + ");
    test_lexer_next_good(lexer, T_INT, "2");
    lex_dispose(lexer);
    
    lexer = lex_init(" 21**34");
    test_lexer_next_good(lexer, T_INT, " 21");
    test_lexer_next_good(lexer, T_POW, "**");
    test_lexer_next_good(lexer, T_INT, "34");
    test_lexer_next_good(lexer, T_END, "\0");
    test_lexer_next_good(lexer, T_ERR, NULL);
    test_lexer_next_good(lexer, T_ERR, NULL);
    lex_dispose(lexer);

    lexer = lex_init("/**-+20 20.5 * *** ");
    test_lexer_next_good(lexer, T_DIV, "/");
    test_lexer_next_good(lexer, T_POW, "**");
    test_lexer_next_good(lexer, T_SUB, "-");
    test_lexer_next_good(lexer, T_ADD, "+");
    test_lexer_next_good(lexer, T_INT, "20");
    test_lexer_next_good(lexer, T_FLT, " 20.5");
    test_lexer_next_good(lexer, T_MUL, " * ");
    test_lexer_next_good(lexer, T_POW, "**");
    test_lexer_next_good(lexer, T_MUL, "* ");
    test_lexer_next_good(lexer, T_END, "\0");
    lex_dispose(lexer);
    
}

static void test_lexer_good(char *source_code) {
    Lex lexer = lex_init(source_code);

    assert_non_null(lexer);
    assert_int_equal(lexer->read_head, 0);
    assert_int_equal(lexer->is_closed, 0);
    assert_non_null(lexer->state_machine);
    assert_string_equal(lexer->source_code, source_code);
    assert_int_equal(lexer->source_length, strlen(source_code));

    lex_dispose(lexer);
}

void test_lexer_next_good(Lex lexer, TokenType exp_type, char *exp_lexeme) {
    Token token = next_token(lexer);
    assert_non_null(token);
    assert_int_equal(token->id, exp_type);
    if (exp_type != T_ERR) {
        assert_non_null(token->lexeme);
        assert_string_equal(token->lexeme, exp_lexeme);
    }
    token_dispose(token);
}