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
	assert_non_null(dfa->states);
	assert_int_equal(dfa->states_count, DFA_STATES_COUNT);
	
	assert_null(dfa->states[0]);
	assert_non_null(dfa->states[1]);
	assert_int_equal(dfa->states[1]->token, I_NUMBER);
	assert_int_equal(dfa->states[1]->value, 2);
	assert_int_not_equal(dfa->states[3]->value, 3);
	assert_null(dfa->states[3]->next_item);
}

void test_get_next_state(void **state) {
	DFA dfa = init_dfa();

	assert_int_equal(get_next_state(dfa, DFA_STATES_COUNT, I_NUMBER), S_ERROR);
	assert_int_equal(get_next_state(NULL, S_ERROR, I_NUMBER), -1);
	assert_int_equal(get_next_state(dfa, S_ERROR, 'x'), S_ERROR);

	assert_int_equal(get_next_state(dfa, S_ERROR, I_NUMBER), S_ERROR);
	assert_int_equal(get_next_state(dfa, S_START, I_SLASH), S_DIVISION);
	assert_int_equal(get_next_state(dfa, S_INTEGER, I_DOT), S_DOT);
	assert_int_equal(get_next_state(dfa, S_DOT, I_NUMBER), S_FLOAT);
	assert_int_equal(get_next_state(dfa, S_FLOAT, I_NUMBER), S_FLOAT);
	assert_int_equal(get_next_state(dfa, S_MULTIPLICATION, I_NUMBER), S_ERROR);
	assert_int_equal(get_next_state(dfa, S_EOD, I_SPACE), S_EOD);
}

void test_is_final_state(void **state) {
	DFA dfa = init_dfa();

	assert_false(is_final_state(dfa, (DfaState)DFA_STATES_COUNT));
	assert_false(is_final_state(dfa, (DfaState)20));
	assert_int_equal(is_final_state(NULL, S_INTEGER), -1);

	assert_true(is_final_state(dfa, S_INTEGER));
	assert_true(is_final_state(dfa, S_FLOAT));
	assert_true(is_final_state(dfa, S_EOD));
	assert_false(is_final_state(dfa, S_ERROR));
	assert_false(is_final_state(dfa, S_START));
	assert_false(is_final_state(dfa, S_DOT));
}
