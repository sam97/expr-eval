#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "lexer/dfa.h"
#include "dfa_tests.h"

void test_dfa_init(void **state) {
	DFA dfa = init_dfa();
	
	assert_non_null(dfa);
	assert_non_null(dfa->final_states);
	assert_non_null(dfa->states);
	assert_int_equal(dfa->states_count, 12);

	assert_int_equal(dfa->final_states[0], 2);
	assert_int_equal(dfa->final_states[8], 11);
	
	assert_null(dfa->states[0]);
	assert_non_null(dfa->states[1]);
	assert_int_equal(dfa->states[1]->token, '0');
	assert_int_equal(dfa->states[1]->value, 2);
	assert_int_not_equal(dfa->states[3]->value, 3);
	assert_null(dfa->states[3]->next_item);
}

void test_get_next_state(void **state) {
	DFA dfa = init_dfa();

	assert_int_equal(get_next_state(dfa, 12, '0'), -1);
	assert_int_equal(get_next_state(NULL, 0, '0'), -1);
	assert_int_equal(get_next_state(dfa, 0, 'x'), 0);

	assert_int_equal(get_next_state(dfa, 0, '0'), 0);
	assert_int_equal(get_next_state(dfa, 1, '/'), 7);
	assert_int_equal(get_next_state(dfa, 2, '.'), 3);
	assert_int_equal(get_next_state(dfa, 3, '0'), 4);
	assert_int_equal(get_next_state(dfa, 4, '0'), 4);
	assert_int_equal(get_next_state(dfa, 5, '0'), 0);
	assert_int_equal(get_next_state(dfa, 11, ' '), 11);
}
