#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "lexer/dfa.h"
#include "dfa_tests.h"

int test_dfa_init() {
	init_dfa();
}