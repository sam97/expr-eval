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
	dfa->states[0] = NULL;
	SETSTATE(1, I_NUMBER, 2, 
		SETSTATE(1, I_DOT, 3, 
			SETSTATE(1, I_ASTERISK, 5, 
				SETSTATE(1, I_SLASH, 7, 
					SETSTATE(1, I_PLUS, 8, 
						SETSTATE(1, I_MINUS, 9, 
							SETSTATE(1, I_NEW_LINE, 10, 
								SETSTATE(1, I_END_OF_STRING, 11, 
									SETSTATE(1, I_SPACE, 1, NULL)))))))));
	SETSTATE(2, I_NUMBER, 2,
		SETSTATE(2, I_DOT, 3, NULL));
	SETSTATE(3, I_NUMBER, 4, NULL);
	SETSTATE(4, I_NUMBER, 4, NULL);
	SETSTATE(5, I_SPACE, 5, NULL);
	SETSTATE(6, I_SPACE, 6, NULL);
	SETSTATE(7, I_SPACE, 7, NULL);
	SETSTATE(8, I_SPACE, 8, NULL);
	SETSTATE(9, I_SPACE, 9, NULL);
	SETSTATE(10, I_SPACE, 10, NULL);
	SETSTATE(11, I_SPACE, 11, NULL);
	
	// Populate the final states
	int final_states[] = DFA_FINAL_STATES;
	dfa->final_states = (int*)malloc(sizeof(final_states));
	if (dfa->final_states == NULL) {
		for (size_t i = 0; i < dfa->states_count; i++)
		{
			DfaEdge edge = dfa->states[i];
			while (edge != NULL) {
				DfaEdge next = edge->next_item;
				free(edge);
				edge = next;
			}
		}
		free(dfa);
		return NULL;
	}

	dfa->final_states_count = (sizeof(final_states)/sizeof(final_states[0]));
	memcpy(dfa->final_states, final_states, sizeof(final_states[0]) * dfa->final_states_count);

	return dfa;
}

int get_next_state(DFA dfa, int current_state, char input) {
	if (!dfa)
		return -1;
	if (current_state >= dfa->states_count)
		return -1;
	
	input = input >= '0' && input <= '9' ? I_NUMBER : input;

	DfaEdge edge = dfa->states[current_state];
	while (edge) {
		if (edge->token == input)
			return edge->value;
		edge = edge->next_item;
	}

	return 0;
}

int is_final_state(DFA dfa, int state) {
	if (!dfa || state >= dfa->states_count)
		return -1;
	
	for (size_t i = 0; i < dfa->final_states_count; i++)
	{
		if (dfa->final_states[i] == state)
			return 1;
	}

	return 0;
}
