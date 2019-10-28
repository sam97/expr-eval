#include "lexer_tests.h"
#include "lexer/lexer.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

static void test_lexer_good(char *source_code);

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

