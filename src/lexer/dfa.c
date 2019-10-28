#include <stdlib.h>
#include <string.h>
#include "dfa.h"

#define SETSTATE(state, token, next_state, next_edge) \
	dfa->states[state] = mkdfa(token, next_state, next_edge)

static DfaEdge mkdfa(char token, int next_state, DfaEdge next_edge) {
	DfaEdge edge = (DfaEdge)malloc(sizeof(struct DfaEdgeListItem));
	if (edge == NULL)
		return NULL;

	edge->token = token;
	edge->value = next_state;
	edge->next_item = next_edge;
	return edge;
}

DFA init_dfa() {
	DFA dfa = (DFA)malloc(sizeof(struct DFAItem));
	if (dfa == NULL)
		return NULL;

	dfa->states_count = DFA_STATES_COUNT;

	// Initialize the states
	dfa->states = (DfaEdge*)malloc(sizeof(DfaEdge) * dfa->states_count);
	if (dfa->states == NULL) {
		free(dfa);
		return NULL;
	}

	// See dfa.h for states, inputs and their definitions.
	dfa->states[S_ERROR] = NULL;
	SETSTATE(S_START, I_NUMBER, S_INTEGER, 
		SETSTATE(S_START, I_DOT, S_DOT, 
			SETSTATE(S_START, I_ASTERISK, S_MULTIPLICATION, 
				SETSTATE(S_START, I_SLASH, S_DIVISION, 
					SETSTATE(S_START, I_PLUS, S_PLUS, 
						SETSTATE(S_START, I_MINUS, S_MINUS, 
							SETSTATE(S_START, I_NEW_LINE, S_NEW_LINE, 
								SETSTATE(S_START, I_END_OF_STRING, S_EOD, 
									SETSTATE(S_START, I_SPACE, S_START, NULL)))))))));
	SETSTATE(S_INTEGER, I_NUMBER, S_INTEGER,
		SETSTATE(S_INTEGER, I_DOT, S_DOT, NULL));
	SETSTATE(S_DOT, I_NUMBER, S_FLOAT, NULL);
	SETSTATE(S_FLOAT, I_NUMBER, S_FLOAT, NULL);
	SETSTATE(S_MULTIPLICATION, I_ASTERISK, S_POWER, 
		SETSTATE(S_MULTIPLICATION, I_SPACE, S_MULT_SPACED, NULL));
	SETSTATE(S_MULT_SPACED, I_SPACE, S_MULT_SPACED, NULL);
	SETSTATE(S_POWER, I_SPACE, S_POWER, NULL);
	SETSTATE(S_DIVISION, I_SPACE, S_DIVISION, NULL);
	SETSTATE(S_PLUS, I_SPACE, S_PLUS, NULL);
	SETSTATE(S_MINUS, I_SPACE, S_MINUS, NULL);
	SETSTATE(S_NEW_LINE, I_SPACE, S_NEW_LINE, NULL);
	SETSTATE(S_EOD, I_SPACE, S_EOD, NULL);
	
	return dfa;
}

int get_next_state(DFA dfa, DfaState current_state, char input) {
	if (!dfa)
		return -1;
	if ((int)current_state >= dfa->states_count)
		return 0;
	
	input = input >= '0' && input <= '9' ? I_NUMBER : input;

	DfaEdge edge = dfa->states[current_state];
	while (edge) {
		if (edge->token == input)
			return edge->value;
		edge = edge->next_item;
	}

	return 0;
}

int is_final_state(DFA dfa, DfaState state) {
	if (!dfa)
		return -1;
	
	switch (state)
	{
	case S_INTEGER:
	case S_FLOAT:
	case S_MULTIPLICATION:
	case S_MULT_SPACED:
	case S_POWER:
	case S_DIVISION:
	case S_PLUS:
	case S_MINUS:
	case S_NEW_LINE:
	case S_EOD:
		return 1;
	default:
		return 0;
	}
}
